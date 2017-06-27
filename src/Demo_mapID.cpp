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

struct imgQueryList{
	vector<string> srcID;
	vector<string> similarID;
};

struct imgQueryList queryList;

//
int getStringIDFromFilePath(
	const string							imageString,
	string									&imageID,
	string									&rectLabel)
{
	string imageIDTemp;
	int sour_pos, postfix_pos;
	string sour_name;
	int rectLabelPos;
	string rectLabelString, rectLabelNew;

	//
	sour_pos 	= imageString.find_last_of('/');
	sour_name	= imageString.substr(sour_pos + 1);
	postfix_pos = sour_name.find_last_of('.');
	imageIDTemp = sour_name.erase(postfix_pos, 4);  
	imageID  = imageIDTemp;

	//labelName
	rectLabelString = imageString.substr(0, sour_pos);	
	rectLabelPos = rectLabelString.find_last_of('/');
	rectLabel = rectLabelString.substr(rectLabelPos + 1);		
	
	//combine the same labels (eg. Armani_1 & Armani_2)
	sour_pos = rectLabel.find_last_of('_');
	rectLabelString = rectLabel.substr(sour_pos + 1);
	if(rectLabelString == "1" || rectLabelString == "2" ){
		rectLabelNew = rectLabel.substr(0, sour_pos);
		rectLabel = rectLabelNew;		
	}

	/*
	sour_pos = imageIDTemp.find_last_of('_');
	imageID  = imageIDTemp.substr(sour_pos + 1);
	*/
	return 0;
}

//
int matchList(
	const string 									dataList,
	const string									svPath)
{
	cout<<"------------------matchList------------------"<<endl;

	int nRet = 0;
	int i;
	char buff[1000];
	string imageID;
	string rectLabel;
	
	int index;
	string srcImgID;	

	FILE* fpDataList = fopen(dataList.c_str(), "r");
    if(NULL == fpDataList){
    	cout<<"cannot open "<<dataList<<endl;
        return -1;
    }

	while(fgets(buff, 1000, fpDataList) != NULL)
	{
		char *pstr = strtok(buff, "\n");
		
		if(pstr != NULL){
			nRet = getStringIDFromFilePath(pstr, imageID, rectLabel);
			if(nRet != 0){
				continue;
			}
			cout<<"imageID = "<<imageID<<endl;
			
			vector<string>::iterator iElement = find( queryList.similarID.begin(), queryList.similarID.end(), imageID);
			index = distance(queryList.similarID.begin(), iElement); 
			
			if(index > -1 && index < 7328){				
				srcImgID = queryList.srcID[index];
				 std::stringstream imgID_temp;
           		 imgID_temp << (pstr);
		   
				std::string dist_path = svPath + rectLabel + "/" + srcImgID + ".jpg";
				cout<<"dist path: "<<dist_path<<endl;
				std::string final_path = "cp "+ imgID_temp.str() + " " + dist_path;
				system(final_path.c_str());
				
			}			
		}
	}
	
    fclose(fpDataList);

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

	char buff[1000];
	int numLines;
	
	queryList.similarID.clear();
	queryList.srcID.clear();
	
	while(fgets(buff, 1000 ,fpQueryList) != NULL)
	{	
		char *pstr = strtok(buff, "\n");		
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
				queryList.srcID.push_back(qstr);
			}else if(numLines == 2){		
				//cout<<"numLabels = "<<numLabel<<endl;
				queryList.similarID.push_back(qstr);
			}

			qstr = strtok(NULL, ",");  
		}		
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
	    "Usage: Demo_mainboby szQueryList similarIDList svPath\n";
	    return 1;
  }	
	
	/***********************************Init**********************************/
	string szQueryList = argv[1];	
	string dataList = argv[2];	
	string svPath = argv[3];
	
	int nRet;	

	nRet = 0;	
	/**************************** imagePredict Deal *************************/
	nRet = getQueryList(szQueryList);
	if(nRet != 0){
		cout<<"fail to getQueryList!"<<endl;
		return -1;
	}

	nRet = matchList(dataList, svPath);
	if(nRet != 0){
		cout<<"fail to matchList!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	return 0;
}

