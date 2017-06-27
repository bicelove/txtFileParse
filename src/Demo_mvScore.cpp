#include <stdio.h>  // for snprintf
#include <string>
#include <vector>
#include <iostream>
#include <sstream> // stringstream
#include <fstream> // NOLINT (readability /streams)
#include <utility> // Create key-value pair (there could be not used)
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <time.h>
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//dirs to save predict patch logo results
int makeDir(
	const string									keyfile,			//[In]: Input dictionary path
	const string									resultPath)			//[In]: Input save results path
{
	string line;
	vector<string> recogClassName;
	string foldname;

	string stringDict = keyfile + "dirDict.txt";

	/**************************************/	
	std::ifstream stringIfs(stringDict);
	if(!stringIfs)
	{
		cout<<"[Open dictionary failed:]"<<stringDict<<endl;
		return -1;
	}
	
	recogClassName.clear();
	while(getline(stringIfs, line))
	{
		if(line.length() != 0){
			recogClassName.push_back(line);
		}
	}
	stringIfs.close();	

	/**************************************/	
	for(int f = 0; f < recogClassName.size(); f++)
	{
		foldname = resultPath + recogClassName[f];
		if(access(foldname.c_str(),0) == -1 && mkdir(foldname.c_str(),0777))
			std::cout<<"create SECOND file bag failed!"<<endl;
	}	
	
	return 0;
}


//
int getStringIDByMvScore(
	const string							imageString,
	string									&imageID,
	string									&rectLabel)
{
	int sour_pos, rectLabelPos;
	int strLen;
	string imageIDTemp, rectLabelString;

	//
	sour_pos 	= imageString.find_last_of('/');
	//cout<<"sour_pos = "<<sour_pos<<endl;	
	imageIDTemp	= imageString.substr(sour_pos + 1);

	//labelName
	rectLabelString = imageString.substr(0, sour_pos);	
	rectLabelPos = rectLabelString.find_last_of('/');
	rectLabel = rectLabelString.substr(rectLabelPos + 1);		

	//
	strLen = imageIDTemp.length();
	sour_pos = imageIDTemp.find_first_of('_');
	if(sour_pos < 0 || sour_pos > (strLen - 3)){
		return -1;
	}
	imageID = imageIDTemp.substr(sour_pos + 1);


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

	int nRet = 0;
	int numth = 0;
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
		if(!img || img->nChannels!=3 || img->depth!=IPL_DEPTH_8U){	
			cout<<"Can't open " << pstr << endl;
			cvReleaseImage(&img);img = 0;
			continue;
		}	

		//save predict results
		sprintf(savePath, "%s/%s/%s", svPath.c_str(), rectLabel.c_str(), srcImgID.c_str());
		cvSaveImage( savePath, img );	
		//cout<<"savePath: "<<savePath<<endl;
		
		numth++;
		if(numth % 100 == 0){
			cout<<"the "<<numth<<"th deal end!"<<endl;
		}
			 
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
	string keyfile = argv[2];
	string svPath = argv[3];	
	
	int nRet = 0;	
	/**************************** imagePredict Deal *************************/
	nRet = makeDir(keyfile, svPath);
	if(nRet != 0){
		cout<<"fail to mv score from imgName!"<<endl;
		return -1;
	}

	//
	nRet = mvScore(dataList, svPath);
	if(nRet != 0){
		cout<<"fail to mv score from imgName!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	return 0;
}
