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

vector<string> queryList;
vector<pair<string, int>> patchList;
vector<pair<string, int>> patchMatchList;

//
int getStringIDFromFilePath(
	const string							imageString,
	string									&imgPath,
	string									&imageID,
	string									&rectLabel,
	float									&score)
{
	int sour_pos, postfix_pos;
	string sour_name;
	int rectLabelPos;
	string scoreStr;

	//
	sour_pos 	= imageString.find_last_of('/');
	//cout<<"sour_pos = "<<sour_pos<<endl;	

	//
	sour_name	= imageString.substr(sour_pos + 1);
	postfix_pos = sour_name.find_last_of('.');
	imageID		= sour_name.erase(postfix_pos, 4);  		

	//labelName
	imgPath 		= imageString.substr(0, sour_pos);	
	rectLabelPos 	= imgPath.find_last_of('/');
	rectLabel 		= imgPath.substr(rectLabelPos + 1);		

	//
	sour_pos 	= imageID.find_first_of('_');
	scoreStr	= imageID.substr(0, sour_pos);
	//cout<<"scoreStr: "<<scoreStr<<endl;
	score 		= atof(scoreStr.c_str());

	return 0;
}

//
int getQueryList(
	const string 									szQueryList)
{
	cout<<"------------------getQueryList------------------"<<endl;
	
	FILE* fpQueryList = fopen(szQueryList.c_str(), "r");
    if(NULL == fpQueryList)
    {
    	cout<<"cannot open "<<szQueryList<<endl;
        return -1;
    }

	int nRet = 0;
	char buff[1000];
	string imgPath;
	string imgLabel;
	string srcImgID;
	string imgTemp;
	float score;
	
	while(fgets(buff, 1000 ,fpQueryList) != NULL)	{	
		
		char *pstr = strtok(buff, "\n");
		imgTemp = pstr;
		
		nRet = getStringIDFromFilePath(pstr, imgPath, srcImgID, imgLabel, score);
		if(nRet != 0){
			continue;
		}	
		//cout<<"srcImgID: "<<srcImgID<<endl;
		//cout<<"score: "<<score<<endl;
		
		if(score >= 0.7){		
			std::string dist_path = imgPath + "/" + imgLabel + "/" + srcImgID + ".jpg";
			std::string final_path = "mv "+ imgTemp + " " + dist_path;
			//cout<<"final_path: "<<final_path<<endl;
			system(final_path.c_str());
		}
	}	
	
    fclose(fpQueryList);	

	return 0;
}

//
int main(int argc, char** argv) {

	const int num_required_args = 2;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts features of an image and predict its label and score.\n"
	    "Usage: Demo_dealPropertyPredict szQueryList\n";
	    return 1;
  }	
	
	/***********************************Init**********************************/
	string szQueryList = argv[1];	
	
	int nRet = 0;	
	
	/**************************** getAllDataQueryList *************************/
	nRet = getQueryList(szQueryList);
	if(nRet != 0){
		cout<<"fail to getQueryList!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	return 0;
}
