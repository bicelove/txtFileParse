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
int getQueryList(
	const string 									szQueryList,
	const string 									outputGrain,
	const string 									outputColor,
	const string 									outputOther)
{
	cout<<"------------------getQueryList------------------"<<endl;
	
	FILE* fpQueryList = fopen(szQueryList.c_str(), "r");
    if(NULL == fpQueryList)
    {
    	cout<<"cannot open "<<szQueryList<<endl;
        return -1;
    }
	
	FILE* fpOutputColor = fopen(outputColor.c_str(), "w");
    if(NULL == fpOutputColor)
    {
    	cout<<"cannot open "<<outputColor<<endl;
        return -1;
    }
	
	FILE* fpOutputGrain = fopen(outputGrain.c_str(), "w");
    if(NULL == fpOutputGrain)
    {
    	cout<<"cannot open "<<outputGrain<<endl;
        return -1;
    }

	FILE* fpOutputOther = fopen(outputOther.c_str(), "w");
    if(NULL == fpOutputOther)
    {
    	cout<<"cannot open "<<outputOther<<endl;
        return -1;
    }
	/*****************************************************/
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
			qstr = strtok(pstr, " ");  
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
				imgID = qstr;
				ifImgID = true;
			}else if(numLines == 2){		
				//cout<<"numLabels str = "<<qstr<<endl;
				label = atoi(qstr);
				//cout<<"label = "<<label<<endl;
				ifLabel = true;
			}

			qstr = strtok(NULL, " ");  
		}		
		
		if(ifImgID && ifLabel)
		{
			if(label == 0){
				fprintf(fpOutputOther, "%s", buffTemp.c_str());
			}else if(label >= 11 && label <= 15){
				fprintf(fpOutputGrain, "%s %d\n", imgID.c_str(), label - 10);
			}else if(label >= 1 && label <= 8){
				fprintf(fpOutputColor, "%s", buffTemp.c_str());
			}
		}	
	}
	
    fclose(fpQueryList);	
	fclose(fpOutputColor);
	fclose(fpOutputGrain);
	fclose(fpOutputOther);
	
	return 0;
}

//
int main(int argc, char** argv) {

	const int num_required_args = 5;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts features of an image and predict its label and score.\n"
	    "Usage: Demo_departPropertyData szQueryList outputGrain outputColor outputOther\n";
	    return 1;
 	}	
	
	/***********************************Init**********************************/
	string szQueryList = argv[1];	
	string outputGrain = argv[2];
	string outputColor = argv[3];
	string outputOther = argv[4];
	
	int nRet;	

	nRet = 0;	
	/**************************** imagePredict Deal *************************/
	nRet = getQueryList(szQueryList, outputGrain, outputColor, outputOther);
	if(nRet != 0){
		cout<<"fail to getQueryList!"<<endl;
		return -1;
	}
	cout<<"deal end!"<<endl;
	
	return 0;
}
