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
	string									&imageID)
{
	int sour_pos;

	//
	sour_pos 	= imageString.find_last_of('/');
	//cout<<"sour_pos = "<<sour_pos<<endl;	
	imageID	= imageString.substr(sour_pos + 1);

	return 0;
}

//
int mvScore(
	const string 									dataList,
	const string									svPath)
{
	FILE* fpQueryList = fopen(dataList.c_str(), "r");
	if(NULL == fpQueryList)
	{
		cout<<"cannot open "<<dataList<<endl;
		return -1;
	}

	FILE* fpSaveList = fopen(svPath.c_str(), "w");
	if(NULL == fpSaveList)
	{
		cout<<"cannot open "<<svPath<<endl;
		return -1;
	}

	int nRet = 0;
	char buff[1000];
	string srcImgID;
	char savePath[256];
	
	while(fgets(buff, 1000 ,fpQueryList) != NULL)
	{	
		char *pstr = strtok(buff, "\n");
		nRet = getStringIDByMvScore(pstr, srcImgID);
		if(nRet != 0){
			continue;
		}

		fprintf(fpSaveList, "%s\n", srcImgID.c_str());
	}

	fclose(fpQueryList);
	fclose(fpSaveList);
	
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
