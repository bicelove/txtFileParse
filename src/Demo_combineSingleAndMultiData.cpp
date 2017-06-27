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
	vector<string>	srcImgIDList;	
	vector<string>	info;
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
	postfix_pos = srcImgID.find_first_of('_');
	
	if(postfix_pos < 0 || postfix_pos >= strLen - 8){
		imageID = srcImgID;
	}else{
		imageID = srcImgID.substr(postfix_pos + 1, strLen - postfix_pos);
	}	
	
	//cout<<"imageID: "<<imageID<<endl;
	
	return 0;
}

//
int getMultiTagDataList(
	const string 									szQueryList)
{
	cout<<"------------------getAllDataQueryList------------------"<<endl;
	
	FILE* fpQueryList = fopen(szQueryList.c_str(), "r");
    if(NULL == fpQueryList)
    {
    	cout<<"cannot open "<<szQueryList<<endl;
        return -1;
    }	

	int i;
	int nRet = 0;
	char buff[2000];
	string srcImgID, imageID, infoTemp;
	bool ifExistImg;

	multiTagDataList.srcImgIDList.clear();	
	multiTagDataList.info.clear();

	while(fgets(buff, 2000 ,fpQueryList) != NULL)
	{	
		infoTemp = buff;
		
		char *pstr = strtok(buff, "\n");
		//cout<<pstr<<endl;
		
		char *qstr = NULL;  
		
		if(pstr != NULL){
			qstr = strtok(pstr, ",");  
		}else{
			continue;
		}

		//cout<<qstr<<endl;
		nRet = getStringID(qstr, srcImgID, imageID);
		if(nRet != 0){
			continue;
		}
		//cout<<"srcImgID: "<<srcImgID<<endl;

		multiTagDataList.srcImgIDList.push_back(srcImgID);
		multiTagDataList.info.push_back(infoTemp);
	}

    fclose(fpQueryList);	

	return 0;
}

//
int tagCombine(
	const string										dataList,
	const string										outputList)
{	
	FILE* fpDataList = fopen(dataList.c_str(), "r");
	if(NULL == fpDataList)
	{
		cout<<"cannot open "<<dataList<<endl;
		return -1;
	}
	FILE* fpOutputList = fopen(outputList.c_str(), "w");
	if(NULL == fpOutputList)
	{
		cout<<"cannot open "<<outputList<<endl;
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
	bool ifImgID, ifImgLabel, ifHaveWrite;

	numIMg = 0;
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
				nRet = getStringID(qstr, srcImgID, imageID);
				if(nRet != 0){
					continue;
				}
				//cout<<"imageID: "<<imageID<<endl;
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
			vector<string>::iterator iElement = find(multiTagDataList.srcImgIDList.begin(), multiTagDataList.srcImgIDList.end(), srcImgID);
			index = distance(multiTagDataList.srcImgIDList.begin(), iElement); 
			
			if(index > -1 && index < multiTagDataList.srcImgIDList.size()){
			//	fprintf(fpOutputList, "%s", multiTagDataList.info[index].c_str());			
			}else{
				fprintf(fpOutputList, "%s,%d\n", srcImgID.c_str(), label);	
			}
			numIMg++;
		}

		if(numIMg % 100 == 0){
			cout<<"the "<<numIMg<<"th image deal!"<<endl;
		}
	}	
	
	cout<<"numIMg = "<<numIMg<<endl;

	fclose(fpDataList);
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

	nRet = getMultiTagDataList(szQueryList);
	if(nRet != 0){
		cout<<"fail to getQueryList!"<<endl;
		return -1;
	}

	cout<<"multiTagDataList.size = "<<multiTagDataList.srcImgIDList.size()<<endl;

	nRet = tagCombine(dataList, outputList);
	if(nRet != 0){
		cout<<"fail to combine tags!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	
	return 0;
}

