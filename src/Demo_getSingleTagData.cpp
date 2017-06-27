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

//
struct imgSingleTagQueryList{
	vector<string>	imgIDList;
	vector<int>		label;
};
struct imgSingleTagQueryList singleTagDataList;

//
struct imgMultiTagQueryList{
	vector<string>	imgIDList;
	vector<vector<int>>	labels;
};
struct imgMultiTagQueryList multiTagDataList;

//
int getMultiTagDataList(
	const string 									szQueryList)
{
	cout<<"------------------getMultiTagDataList------------------"<<endl;
	
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
	vector<int> labels;
	bool ifImgID, ifImgLabel, ifSingleTagImg;
	
	multiTagDataList.imgIDList.clear();
	multiTagDataList.labels.clear();

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
		labels.clear();
		while(qstr != NULL)
		{
			ifSingleTagImg = true;
			numLines++;
			if(numLines == 1){
				//cout<<qstr<<endl;
				imgID = qstr;
				ifImgID = true;
			}else if(numLines == 2){
				//cout<<qstr<<endl;
				label = atoi(qstr);
				labels.push_back(label);
				ifImgLabel = true;
			}else if(numLines > 2){				
				label = atoi(qstr);
				labels.push_back(label);
				ifSingleTagImg = false;
			}			
			qstr = strtok(NULL, ",");  		
		}		

		if(!ifSingleTagImg && labels.size()>1 && ifImgID && ifImgLabel)
		{
			multiTagDataList.imgIDList.push_back(imgID);
			multiTagDataList.labels.push_back(labels);
		}	
	}

    fclose(fpQueryList);	

	return 0;
}

//
int getSingleTagDataList(
	const string 									szQueryList)
{
	cout<<"------------------getSingleTagDataList------------------"<<endl;
	
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
int outputSingleMatchList(
	const string										outputList)
{	
	if(singleTagDataList.imgIDList.size() != singleTagDataList.label.size()){
		cout<<"image information error!"<<endl;
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
	
	for(i = 0; i < singleTagDataList.imgIDList.size(); i++)
	{
		fprintf(fpOutputList, "%s\n", singleTagDataList.imgIDList[i].c_str());
	}
	
	fclose(fpOutputList);	

	return 0;
}


//
int outputMUltiMatchList(
	const string										outputList,
	const string										outputMultiTagClassLabelList)
{	
	if(multiTagDataList.imgIDList.size() != multiTagDataList.labels.size()){
		cout<<"image information error!"<<endl;
		return -1;
	}
	
	FILE* fpOutputList = fopen(outputList.c_str(), "w");
	if(NULL == fpOutputList)
	{
		cout<<"cannot open "<<outputList<<endl;
		return -1;
	}

	FILE* fpOutputClassLabelList = fopen(outputMultiTagClassLabelList.c_str(), "w");
	if(NULL == fpOutputList)
	{
		cout<<"cannot open "<<fpOutputClassLabelList<<endl;
		return -1;
	}

	/********************************************************/
	int i, j;
	
	for(i = 0; i < multiTagDataList.imgIDList.size(); i++)
	{
		//
		fprintf(fpOutputList, "%s,", multiTagDataList.imgIDList[i].c_str());
		fprintf(fpOutputClassLabelList, "%s,", multiTagDataList.imgIDList[i].c_str());
		
		for(j = 0; j < multiTagDataList.labels[i].size(); j++)
		{		
			fprintf(fpOutputList, "%d,", multiTagDataList.labels[i][j]);

			if(multiTagDataList.labels[i][j] > 13){
				fprintf(fpOutputClassLabelList, "%d,", multiTagDataList.labels[i][j]);
			}
		}		
		fprintf(fpOutputList, "\n");
		fprintf(fpOutputClassLabelList, "\n");
	}
	
	fclose(fpOutputList);	
	fclose(fpOutputClassLabelList);
	
	return 0;
}

//
int main(int argc, char** argv) {

	const int num_required_args = 5;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts features of an image and predict its label and score.\n"
	    "Usage: Demo_getSingleTagData szQueryList outputSingleTagList outputMultiTagList outputMultiTagClassLabelList\n";
	    return 1;
  }	
	
	/***********************************Init**********************************/
	string szQueryList = argv[1];	
	string outputSingleTagList = argv[2];
	string outputMultiTagList = argv[3];
	string outputMultiTagClassLabelList = argv[4];
	
	int nRet = 0;		
	/**************************** getAllDataQueryList *************************/
	nRet = getSingleTagDataList(szQueryList);
	if(nRet != 0){
		cout<<"fail to getSingleTagDataList!"<<endl;
		return -1;
	}
	nRet = getMultiTagDataList(szQueryList);
	if(nRet != 0){
		cout<<"fail to getMultiTagDataList!"<<endl;
		return -1;
	}

	//
	nRet = outputSingleMatchList(outputSingleTagList);
	if(nRet != 0){
		cout<<"fail to output match patch list!"<<endl;
		return -1;
	}

	//
	nRet = outputMUltiMatchList(outputMultiTagList, outputMultiTagClassLabelList);
	if(nRet != 0){
		cout<<"fail to output match patch list!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	return 0;
}
