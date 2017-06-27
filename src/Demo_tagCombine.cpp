#include <stdio.h>  // for snprintf
#include <string>
#include <vector>
#include <iostream>
#include <sstream> // stringstream
#include <fstream> // NOLINT (readability /streams)
#include <utility> // Create key-value pair (there could be not used)
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

struct imgSingleQueryList{
	vector<string>	imgIDList;
	vector<int>		label;
};
struct imgSingleQueryList singleTagDataList;

struct imgMultiQueryList{
	vector<string>		imgIDList;
	vector<vector<int>>	labels;
};
struct imgMultiQueryList multiTagDataList;

//
int getStringID(
	const string							imageString,
	string 									&srcImgID,
	string									&imageID)
{
	int sour_pos, postfix_pos;
	string sour_name;
	
	//
	sour_pos  = imageString.find_last_of('/');
	sour_name = imageString.substr(sour_pos + 1);
	srcImgID = sour_name;
	//cout<<"nameTemp1: "<<nameTemp1<<endl;

	int strLen = srcImgID.length();
	postfix_pos = srcImgID.find_last_of('_');
	
	if(postfix_pos < 0 || postfix_pos >= strLen - 6){
		imageID = srcImgID;
	}else{
		imageID = srcImgID.substr(postfix_pos + 1, strLen - postfix_pos);
	}	
	
	//cout<<"imageID: "<<imageID<<endl;
	
	return 0;
}

//
int getSingleTagDataList(
	const string 									szQueryList)
{
	cout<<"------------------getAllDataQueryList------------------"<<endl;
	
	FILE* fpQueryList = fopen(szQueryList.c_str(), "r");
    if(NULL == fpQueryList)
    {
    	cout<<"cannot open "<<szQueryList<<endl;
        return -1;
    }	

	char buff[2000];
	int numLines;	
	string imgID;
	int label;
	bool ifImgID, ifImgLabel, ifSingleTagImg;
	
	singleTagDataList.imgIDList.clear();
	singleTagDataList.label.clear();

	while(fgets(buff, 2000 ,fpQueryList) != NULL)
	{	
		char *pstr = strtok(buff, "\n");
		//cout<<pstr<<endl;
		
		char *qstr = NULL;  
		
		if(pstr != NULL){
			qstr = strtok(pstr, ",");  
		}else{
			continue;
		}
				
		numLines = 0;
		ifImgID = false;
		ifImgLabel = false;
		ifSingleTagImg = true;
		while(qstr != NULL)
		{
			numLines++;
			if(numLines == 1){
				//cout<<qstr<<endl;
				imgID = qstr;
				ifImgID = true;
			}else if(numLines == 2){
				//cout<<qstr<<endl;
				label = atoi(qstr);
				ifImgLabel = true;
			}else if(numLines > 2){
				ifSingleTagImg = false;
				break;
			}			
			qstr = strtok(NULL, ",");  		
		}		

		if(ifSingleTagImg && ifImgID && ifImgLabel)
		{
			singleTagDataList.imgIDList.push_back(imgID);
			singleTagDataList.label.push_back(label);
		}		
	}

    fclose(fpQueryList);	

	return 0;
}

//
int tagCombine(
	const string										dataList)
{	
	FILE* fpDataList = fopen(dataList.c_str(), "r");
	if(NULL == fpDataList)
	{
		cout<<"cannot open "<<dataList<<endl;
		return -1;
	}

	int i;	
	int nRet = 0;
	char buff[2000];

	string imgPath;
	string srcImgID;
	string imageID;
	int label;
	int index;
	int numIMg;
	int numLines;	
	bool ifImgID, ifImgLabel;
	vector<pair<int, bool>> ifwrite(singleTagDataList.imgIDList.size(), make_pair(-1, false));

	multiTagDataList.imgIDList.clear();
	multiTagDataList.labels.resize(singleTagDataList.label.size());

	numIMg = 0;
	ifwrite.clear();
	while(fgets(buff, 2000 ,fpDataList) != NULL)
	{	
		char *pstr = strtok(buff, "\n");
		//cout<<pstr<<endl;
		char *qstr = NULL;	
		
		if(pstr != NULL){
			qstr = strtok(pstr, ",");  
		}else{
			continue;
		}
				
		numLines = 0;
		ifImgID = false;
		ifImgLabel = false;
		while(qstr != NULL)
		{
			numLines++;
			if(numLines == 1){
				//cout<<qstr<<endl;
				imgPath = qstr;
				nRet = getStringID(pstr, srcImgID, imageID);
				if(nRet != 0){
					continue;
				}
				ifImgID = true;
			}else if(numLines == 2){
				//cout<<qstr<<endl;
				label = atoi(qstr);
				ifImgLabel = true;
			}else if(numLines > 2){
				break;
			}			
			qstr = strtok(NULL, ",");  		
		}		

		if(ifImgID && ifImgLabel)
		{				
			//cout<<"imageID: "<<imageID<<endl;
			vector<string>::iterator iElement = find(singleTagDataList.imgIDList.begin(), singleTagDataList.imgIDList.end(), imageID);
			index = distance(singleTagDataList.imgIDList.begin(), iElement); 
			if(index > -1 && index < singleTagDataList.imgIDList.size()){
				if(!ifwrite[index].second){
					multiTagDataList.imgIDList.push_back(singleTagDataList.imgIDList[index]);
					numIMg++;
					multiTagDataList.labels[numIMg - 1].push_back(singleTagDataList.label[index]);
					multiTagDataList.labels[numIMg - 1].push_back(label);
					ifwrite[index].first = numIMg - 1;
					ifwrite[index].second = true;
				}else{					
					multiTagDataList.labels[ifwrite[index].first].push_back(label);
				}
			}	
		}

		if(numIMg % 100 == 0){
			cout<<"the "<<numIMg<<"th image deal!"<<endl;
		}
	}	
	
	cout<<"numIMg = "<<numIMg<<endl;

	fclose(fpDataList);
	
	return 0;
}

//
int outputMatchList(
	const string										outputList)
{	
	if(multiTagDataList.imgIDList.size() < 1 || multiTagDataList.labels.size() < 1){
		cout<<"image information error!"<<endl;
		return -1;
	}

	FILE* fpOutputList = fopen(outputList.c_str(), "w");
	if(NULL == fpOutputList)
	{
		cout<<"cannot open "<<outputList<<endl;
		return -1;
	}

	int i, j;	
	int nRet = 0;

	int label;
	int index;
	int numIMg;

	numIMg = 0;
	for(i = 0; i < multiTagDataList.imgIDList.size(); i++)
	{	
		numIMg++;		
		fprintf(fpOutputList, "%s,", multiTagDataList.imgIDList[i].c_str());
		for(j = 0; j < multiTagDataList.labels[i].size(); j++){
			fprintf(fpOutputList, "%d,", multiTagDataList.labels[i][j]);
		}		
		fprintf(fpOutputList, "\n");
	}	
	
	cout<<"numIMg = "<<numIMg<<endl;

	fclose(fpOutputList);
	
	return 0;
}

//
int main(int argc, char** argv) {

	const int num_required_args = 3;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts features of an image and predict its label and score.\n"
	    "Usage: Demo_mainboby szQueryList outputList\n";
	    return 1;
  }	
	
	/***********************************Init**********************************/
	string szQueryList = argv[1];		
	string dataList = argv[2];	
	string outputList = argv[3];
	
	int nRet = 0;		
	/**************************** getAllDataQueryList *************************/		

	nRet = getSingleTagDataList(szQueryList);
	if(nRet != 0){
		cout<<"fail to getQueryList!"<<endl;
		return -1;
	}

	cout<<"singleTagDataList.size = "<<singleTagDataList.imgIDList.size()<<endl;

	nRet = tagCombine(dataList);
	if(nRet != 0){
		cout<<"fail to combine tags!"<<endl;
		return -1;
	}
	
	nRet = outputMatchList(outputList);
	if(nRet != 0){
		cout<<"fail to output match list!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	
	return 0;
}

