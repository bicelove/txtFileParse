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

struct imgMultiQueryList{
	vector<string> imgIDList;
	vector<vector<int>>	labels;
};

vector<pair<int,vector<string>>> queryList;
struct imgMultiQueryList multiList;

//
int getStringID(
	const string							imageString,
	string 									&srcImgID)
{
	int sour_pos;
	string sour_name;
	
	//
	sour_pos  = imageString.find_last_of('/');
	sour_name = imageString.substr(sour_pos + 1);
	srcImgID = sour_name;

	return 0;
}

//
int getQueryList(
	const string 									szQueryList,
	const int 										labelNum)
{
	cout<<"------------------getEachLabelQueryList------------------"<<endl;
	
	FILE* fpQueryList = fopen(szQueryList.c_str(), "r");
    if(NULL == fpQueryList)
    {
    	cout<<"cannot open "<<szQueryList<<endl;
        return -1;
    }	

	int nRet = 0;
	char buff[2000];
	
	int numLines;	
	string imgID;
	int label;
	bool ifImgID, ifImgLabel;
	vector<string> imgIDList;

	imgIDList.clear();
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
		while(qstr != NULL)
		{
			numLines++;
			if(numLines == 1){
				//cout<<qstr<<endl;
				nRet = getStringID(qstr, imgID);
				if(nRet != 0)continue;
				ifImgID = true;
			}else if(numLines == 2){
				//cout<<qstr<<endl;
				label = atoi(qstr);
				if(label != labelNum)continue;
				ifImgLabel = true;
			}else if(numLines > 2){
				break;
			}			
			qstr = strtok(NULL, ",");  		
		}		

		if(ifImgID && ifImgLabel)
		{
			imgIDList.push_back(imgID);
		}		
	}

	queryList.push_back(make_pair(labelNum, imgIDList));

    fclose(fpQueryList);	

	return 0;
}

//
int GetMultiList(
	const string										IDList)
{
	FILE* fpDataList = fopen(IDList.c_str(), "r");
    if(NULL == fpDataList){
    	cout<<"cannot open "<<IDList<<endl;
        return -1;
    }
	
	int i;	
	int nRet = 0;
	char buff[1000];
	string imgID;
	int numLines;
	int label;
	bool ifImgID, ifImgLabel;
	vector<int> labels;
	
	multiList.imgIDList.clear();
	multiList.labels.clear();
	
	while(fgets(buff, 1000, fpDataList) != NULL)
	{		
		char *pstr = strtok(buff, "\n");	
		char *qstr = NULL;  
		
		if(pstr != NULL){
			qstr = strtok(pstr, ",");  
		}else{
			continue;
		}		
		
		numLines = 0;
		ifImgID = false;
		labels.clear();
		while(qstr != NULL)
		{
			ifImgLabel = false;
			numLines++;
			if(numLines == 1){
				//cout<<qstr<<endl;
				nRet = getStringID(qstr, imgID);
				if(nRet != 0)continue;
				ifImgID = true;
			}else if(numLines >= 2){
				//cout<<qstr<<endl;
				label = atoi(qstr);
				labels.push_back(label);
				ifImgLabel = true;
			}
			
			qstr = strtok(NULL, ",");  		
		}		

		if(ifImgID && ifImgLabel)
		{
			multiList.imgIDList.push_back(imgID);
			multiList.labels.push_back(labels);
		}		

	}
	
	fclose(fpDataList);

	return 0;
}

//
int outputUnmatchList(
	const string										outputList)
{
	if(queryList.size() < 1 ||  queryList.size() > 15){
		cout<<"query List size error!\n queryList.size = "<<queryList.size()<<endl;
		return -1;
	}

	if(multiList.imgIDList.size() != multiList.labels.size()){
		cout<<"multi List error!"<<endl;
		cout<<"multiList.imgIDList.size = "<<multiList.imgIDList.size()<<"; ";
		cout<<"multiList.labels.size = "<<multiList.labels.size()<<endl;
		return -1;
	}
	
	FILE* fpOutputList = fopen(outputList.c_str(), "w");
	if(NULL == fpOutputList)
	{
		cout<<"cannot open "<<outputList<<endl;
		return -1;
	}

	int i, j, k;	
	int nRet = 0;
	string srcImgID;
	vector<string> imgIDTemp;
	int index;
	bool ifCorrect;

	for(i = 0; i < multiList.imgIDList.size(); i++)
	{		
		srcImgID = multiList.imgIDList[i];
		ifCorrect = false;
		imgIDTemp.clear();
		for(j = 0; j < queryList.size(); j++)
		{
			imgIDTemp = queryList[j].second;

			vector<string>::iterator iElement = find(imgIDTemp.begin(), imgIDTemp.end(), srcImgID);
			index = distance(imgIDTemp.begin(), iElement); 
			if(index > -1 && index < imgIDTemp.size())
			{
				//cout<<"id: "<<imgIDTemp[index]<<endl;

				for(k = 0; k < multiList.labels[i].size(); k++){				
					if(	queryList[j].first == multiList.labels[i][k]){
						ifCorrect = true;
						cout<<"correct!"<<endl;
						break;
					}
				}
				if(!ifCorrect){
					fprintf(fpOutputList, "%s,%d\n", srcImgID.c_str(), 	queryList[j].first);
				}
				
				break;
			}
		}		
	}
	
	fclose(fpOutputList);	

	return 0;
}

//
int main(int argc, char** argv) {

	const int num_required_args = 4;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts features of an image and predict its label and score.\n"
	    "Usage: Demo_mainboby szQueryList IDList outputList\n";
	    return 1;
  }		
	
	/***********************************Init**********************************/
	string szQueryList = argv[1];	
	string IDList = argv[2];	
	string outputList = argv[3];
	
	int nRet;	
	char buff[1000];
	nRet = 0;	
	int labelNum = 14;
	
	/**************************** getAllDataQueryList *************************/
	FILE* fpDataList = fopen(szQueryList.c_str(), "r");
    if(NULL == fpDataList){
    	cout<<"cannot open "<<IDList<<endl;
        return -1;
    }
	
	queryList.clear();
	while(fgets(buff, 1000, fpDataList) != NULL)
	{
		char *pstr = strtok(buff, "\n");	//imgID,imgLabel
		nRet = getQueryList(pstr, labelNum);
		if(nRet != 0){
			cout<<"fail to getQueryList!"<<endl;
			return -1;
		}
		labelNum++;
	}
	fclose(fpDataList);

	//
	nRet = GetMultiList(IDList);
	if(nRet != 0){
		cout<<"fail to match patch list!"<<endl;
		return -1;
	}
	
	nRet = outputUnmatchList(outputList);
	if(nRet != 0){
		cout<<"fail to output match patch list!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	return 0;
}
