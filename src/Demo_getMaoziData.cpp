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
int getStringID(
	const string							imageString,
	string 									&srcImgID)
{
	int sour_pos;
	
	//
	sour_pos = imageString.find_last_of('/');
	srcImgID = imageString.substr(sour_pos + 1);

	return 0;
}

//
int outputMatchList(
	const string										srcDataList,
	const string										outputDataList)
{	
	FILE* fpSrcData = fopen(srcDataList.c_str(), "r");
	if(NULL == fpSrcData)
	{
		cout<<"cannot open "<<srcDataList<<endl;
		return -1;
	}

	FILE* fpOutputList = fopen(outputDataList.c_str(), "w");
	if(NULL == fpOutputList)
	{
		cout<<"cannot open "<<outputDataList<<endl;
		return -1;
	}

	/********************************************************/
	int i, j;
	
	int nRet;
	char buff[2000];
	int numLines;	
	string imgID;
	int label;
	bool ifImgID, ifImgLabel;
	string buffTemp;
	int index;
	
	while(fgets(buff, 2000, fpSrcData) != NULL)
	{
		buffTemp = buff;
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
				if(nRet != 0)break;
				ifImgID = true;
			}else if(numLines >= 2){
				//cout<<qstr<<endl;
				label = atoi(qstr);
				if(label == 23){
					ifImgLabel = true;
					break;
				}
			}	
			qstr = strtok(NULL, ",");  		
		}		

		if(ifImgID && ifImgLabel)
		{				
			fprintf(fpOutputList, "%s", buffTemp.c_str());
		}
	}
	
	fclose(fpSrcData);	
	fclose(fpOutputList);
	
	return 0;
}

//
int main(int argc, char** argv) {

	const int num_required_args = 3;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts Maozi data.\n"
	    "Usage: Demo_getMaoziData.sh srcDataList outputDataList\n";
	    return 1;
  	}	
	
	/***********************************Init**********************************/
	string srcDataList = argv[1];
	string outputDataList = argv[2];
	
	int nRet = 0;		

	//
	nRet = outputMatchList(srcDataList, outputDataList);
	if(nRet != 0){
		cout<<"fail to output match patch list!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	return 0;
}
