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
int mvJpgToLabel(
	const string									srcImgPath,
	const string									saveImgPath,
	const string									imgPath,
	const int										label,
	const long long									imgName)
{
	char savePath[256];
	string loadImgPath;
	
	loadImgPath = srcImgPath + "/" + imgPath;
	
	IplImage * img = cvLoadImage(loadImgPath.c_str());
	if(!img || img->nChannels!=3 || img->depth!=IPL_DEPTH_8U) 
	{	
		cout<<"Can't open " << loadImgPath << endl;
		if(img){cvReleaseImage(&img); img = 0;}
		return -1;
	}
	
	sprintf(savePath, "%s/%d/%lld.jpg", saveImgPath.c_str(), label, imgName);
	cvSaveImage(savePath, img);	
	if(img){cvReleaseImage(&img); img = 0;}
		
	return 0;
}
	
//
int getQueryList(
	const string 									szQueryList,
	const string 									srcImgPath,
	const string 									saveImgPath)
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
	long long imgName;
		
	int numLines;	
	string imgID;
	int label;
	bool ifImgID, ifImgLabel;

	imgName = 20170217000000;
	while(fgets(buff, 2000 ,fpQueryList) != NULL)
	{	
		char *pstr = strtok(buff, "\n");
		//cout<<pstr<<endl;
		
		char *qstr = NULL;  
		
		if(pstr != NULL){
			qstr = strtok(pstr, "\t ");  
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
				/*
				nRet = getStringID(qstr, imgID);
				if(nRet != 0)continue;
				*/
				imgID = qstr;
				ifImgID = true;
			}else if(numLines == 2){
				//cout<<qstr<<endl;
				label = atoi(qstr);
				ifImgLabel = true;
			}else if(numLines > 2){
				break;
			}			
			qstr = strtok(NULL, " ");  		
		}		

		imgName++;
		nRet = mvJpgToLabel(srcImgPath, saveImgPath, imgID, label, imgName);
		if(nRet != 0)continue;
	}
	
    fclose(fpQueryList);	

	return 0;
}

//
int main(int argc, char** argv) {

	const int num_required_args = 4;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts features of an image and predict its label and score.\n"
	    "Usage: Demo_mainboby szQueryList srcImgPath saveImgPath\n";
	    return 1;
  }		
	
	/***********************************Init**********************************/
	string szQueryList = argv[1];	
	string srcImgPath = argv[2];	
	string saveImgPath = argv[3];
	
	int nRet;	
	nRet = 0;		
	/**************************** getAllDataQueryList *************************/
	nRet = getQueryList(szQueryList, srcImgPath, saveImgPath);
	if(nRet != 0){
		cout<<"fail to getQueryList!"<<endl;
		return -1;
	}

	cout<<"deal end!"<<endl;
	
	return 0;
}
