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
int getStringIDFromFilePath(
	const string							imageString,
	string									&imageID)
{
	string imageIDTemp;
	int sour_pos, postfix_pos;
	string sour_name;
	int rectLabelPos;
	string rectLabelString, rectLabelNew;

	//
	sour_pos 	= imageString.find_last_of('/');
	//cout<<"sour_pos = "<<sour_pos<<endl;	
	sour_name	= imageString.substr(sour_pos + 1);
	postfix_pos = sour_name.find_last_of('.');
	imageIDTemp = sour_name.erase(postfix_pos, 4);  		

	//
	imageID  = imageIDTemp;

	return 0;
}

//
int makeMultiTagDirList(
	const string 									szQueryList,
	const string 									svPath)
{
	cout<<"------------------getMultiTagDataList------------------"<<endl;
	
	FILE* fpQueryList = fopen(szQueryList.c_str(), "r");
    if(NULL == fpQueryList)
    {
    	cout<<"cannot open "<<szQueryList<<endl;
        return -1;
    }	

	int nRet = 0;
	char buff[2000];
	char savePath[200];
	int numLines;	
	string imgID, srcImgID;
	int label;
	
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
		while(qstr != NULL)
		{
			numLines++;
			if(numLines == 1){
				//cout<<qstr<<endl;
				imgID = qstr;
				nRet = getStringIDFromFilePath(imgID, srcImgID);
			}else if(numLines >= 2){
				//cout<<qstr<<endl;
				label = atoi(qstr);
				if(label >= 1 && label <= 13){
					std::stringstream labelTemp;
		            labelTemp << (label);
					//sprintf(savePath, "%s/%d/%s.jpg", svPath.c_str(), label, srcImgID.c_str());
					std::string dist_path = svPath + "/" + labelTemp.str() + "/" + srcImgID + ".jpg";
			        std::string final_path = "cp "+ imgID + " " + dist_path;
			        system(final_path.c_str());
				}
			}		
			qstr = strtok(NULL, ",");  		
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
	    "Usage: Demo_mkDataDir szQueryList svPath\n";
	    return 1;
	}	
	
	/***********************************Init**********************************/
	string szQueryList = argv[1];	
	string svPath = argv[2];
	
	int nRet = 0;		
	/**************************** getAllDataQueryList *************************/
	nRet = makeMultiTagDirList(szQueryList, svPath);
	if(nRet != 0){
		cout<<"fail to makeMultiTagDirList!"<<endl;
		return -1;
	}

	cout<<"deal end!"<<endl;
	
	return 0;
}
