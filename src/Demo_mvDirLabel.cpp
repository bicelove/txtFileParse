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
int getStringIDByMvScore(
	const string							imageString,
	string									&imageID,
	string									&rectLabel)
{
	string imageIDTemp;
	int sour_pos, postfix_pos;
	string sour_name;
	int rectLabelPos;
	string rectLabelString;
	int strLen;

	//
	sour_pos 	= imageString.find_last_of('/');
	//cout<<"sour_pos = "<<sour_pos<<endl;	
	sour_name	= imageString.substr(sour_pos + 1);
	postfix_pos = sour_name.find_last_of('.');
	imageIDTemp = sour_name.erase(postfix_pos, 4);  	

	//labelName
	rectLabelString = imageString.substr(0, sour_pos);	
	rectLabelPos = rectLabelString.find_last_of('/');
	rectLabel = rectLabelString.substr(rectLabelPos + 1);	

	//
	//while(1) //delete all scores
	{
		strLen = imageIDTemp.length();
		sour_pos = imageIDTemp.find_first_of('_');
		if(sour_pos < 0 || sour_pos > (strLen - 3)){
			//break;
			return -1;
		}
		imageIDTemp = imageIDTemp.substr(sour_pos + 1);
	}
		
	//
	imageID = imageIDTemp;

	return 0;
}

//
int mvScore(
	const string 									imgID,
	const string									svPath)
{
	FILE* fpQueryList = fopen(imgID.c_str(), "r");
	if(NULL == fpQueryList)
	{
		cout<<"cannot open "<<imgID<<endl;
		return -1;
	}

	int nRet = 0;
	char buff[1000];
	string srcImgID, rectLabel;
	char savePath[256];
	
	while(fgets(buff, 1000 ,fpQueryList) != NULL)
	{	
		char *pstr = strtok(buff, "\n");
		nRet = getStringIDByMvScore(pstr, srcImgID, rectLabel);
		if(nRet != 0){
			continue;
		}

		IplImage *img = cvLoadImage(pstr);
		if(!img || img->nChannels!=3 || img->depth!=IPL_DEPTH_8U) 
		{	
			cout<<"Can't open " << pstr << endl;
			cvReleaseImage(&img);img = 0;
			continue;
		}

		sprintf(savePath, "%s/%s/%s.jpg", svPath.c_str(), rectLabel.c_str(), srcImgID.c_str());
		cvSaveImage( savePath, img );

		cvReleaseImage(&img);img = 0;
	}

	fclose(fpQueryList);
	
	return 0;
}

//
int main(int argc, char** argv) {

	const int num_required_args = 3;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts features of an image and predict its label and score.\n"
	    "Usage: Demo_mainboby szQueryList similarIDList srcIDList outputList\n";
	    return 1;
  }	
	
	/***********************************Init**********************************/
	string dataList = argv[1];	
	string svPath = argv[2];	
	
	int nRet = 0;	
	/**************************** imagePredict Deal *************************/
	nRet = mvScore(dataList, svPath);
	if(nRet != 0){
		cout<<"fail to mv score from imgName!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	return 0;
}
