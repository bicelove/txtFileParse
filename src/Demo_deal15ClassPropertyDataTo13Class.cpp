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
	const string 									outputList)
{
	cout<<"------------------getQueryList------------------"<<endl;
	
	FILE* fpQueryList = fopen(szQueryList.c_str(), "r");
    if(NULL == fpQueryList)
    {
    	cout<<"cannot open "<<szQueryList<<endl;
        return -1;
    }
	
	FILE* fpOutputList = fopen(outputList.c_str(), "w");
    if(NULL == fpOutputList)
    {
    	cout<<"cannot open "<<outputList<<endl;
        return -1;
    }

	/*****************************************************/
	int nRet = 0;
	char buff[2000];
	int numLines;
	int	labelIndex;
	
	string imgID, buffTemp;
	int label;

	bool ifImgID, ifLabel;

	/*****************************************************/
	while(fgets(buff, 2000 ,fpQueryList) != NULL)
	{	
		buffTemp = buff;
		char *pstr = strtok(buff, "\n");
		//cout<<"pstr1: "<<pstr<<endl;
		
		char *qstr = NULL;  
		
		if(pstr != NULL){
			qstr = strtok(pstr, ",");  
		}else{
			continue;
		}		
		//cout<<"pstr2: "<<pstr<<endl;
				
		numLines = 0;
		ifImgID = false;
		ifLabel = false;
		while(qstr != NULL)
		{
			numLines++;
			if(numLines == 1){
				nRet = getStringID(qstr, imgID);
				ifImgID = true;
			}else if(numLines == 2){		
				//cout<<"numLabels str = "<<qstr<<endl;
				label = atoi(qstr);
				//cout<<"label = "<<label<<endl;
				ifLabel = true;
			}

			qstr = strtok(NULL, ",");  
		}		
		
		if(ifImgID && ifLabel)
		{
			if(label >= 1 && label <= 8){
				fprintf(fpOutputList, "%s,%d\n", imgID.c_str(), label);
			}else if(label >= 11 && label <= 15){
				fprintf(fpOutputList, "%s,%d\n", imgID.c_str(), label - 2);
			} 
		}	
	}
	
    fclose(fpQueryList);	
	fclose(fpOutputList);
	
	return 0;
}

//
int main(int argc, char** argv) {

	const int num_required_args = 3;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts features of an image and predict its label and score.\n"
	    "Usage: Demo_deal15ClassPropertyDataTo13Class szQueryList outputList\n";
	    return 1;
 	}	
	
	/***********************************Init**********************************/
	string szQueryList = argv[1];	
	string outputList = argv[2];
	
	int nRet;	

	nRet = 0;	
	/**************************** imagePredict Deal *************************/
	nRet = getQueryList(szQueryList, outputList);
	if(nRet != 0){
		cout<<"fail to getQueryList!"<<endl;
		return -1;
	}
	cout<<"deal end!"<<endl;
	
	return 0;
}
