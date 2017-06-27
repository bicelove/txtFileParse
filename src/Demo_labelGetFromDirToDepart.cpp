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
int getPath(
	const string							imageString,
	string									&sourPath,
	int 									&imgLabel)
{
	int sour_pos;
	string imgLabelString;
	
	//
	sour_pos = imageString.find_first_of('-');
	cout<<"sour_pos: "<<sour_pos<<endl;
	
	imgLabelString = imageString.substr(0, sour_pos);
	sourPath = imageString.substr(sour_pos + 1);
	cout<<"sourPath: "<<sourPath<<endl;
	
	imgLabel = atoi(imgLabelString.c_str());
		
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

	/*******************************************************/
	int nRet = 0;	
	string imgPath, srcName;
	int imgLabel;
	char buff[1000];
			
	while(fgets(buff, 1000, fpQueryList) != NULL){		
		imgPath = strtok(buff, "\n");
		
		nRet = getPath(imgPath, srcName, imgLabel);
		if(nRet != 0){
			cout<<"get image path error!"<<endl;
			continue;
		}		
		cout<<"imgLabel: "<<imgLabel<<endl;

		fprintf(fpOutputList, "%d,%s\n", imgLabel, srcName.c_str());
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
	    "Usage: Demo_labelGetFromDir szQueryList outputList\n";
	    return 1;
 	}		
	
	/********************************* Init **********************************/
	string szQueryList = argv[1];	
	string outputList = argv[2];
	
	int nRet;	
	
	/**************************** getAllDataQueryList *************************/
	nRet = 0;	
	nRet = getQueryList(szQueryList, outputList);
	if(nRet != 0){
		cout<<"fail to getQueryList!"<<endl;
		return -1;
	}
			
	cout<<"deal end!"<<endl;
	
	return 0;
}
