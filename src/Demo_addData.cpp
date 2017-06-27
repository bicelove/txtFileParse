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

struct imgQueryList{
	vector<string>	imgIDList;
	vector<int>		label;
};
struct imgQueryList singleTagDataList;
FILE* fpOutputList = NULL;

//
int getStringIDFromFilePath(
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
int getSingleTagDataList(
	const string 									szQueryList)
{
	cout<<"------------------getAllDataQueryList------------------"<<endl;
	
	FILE* fpQueryList = fopen(szQueryList.c_str(), "r");
    if(NULL == fpQueryList)
    {
    	cout<<"cannot open "<<szQueryList<<endl;
        return -1;
    }	

	char buff[2000];
	string buffTemp;
	int numLines;	
	string imgID;
	int label;
	bool ifImgID, ifImgLabel, ifSingleTagImg;
	
	singleTagDataList.imgIDList.clear();
	singleTagDataList.label.clear();

	while(fgets(buff, 2000 ,fpQueryList) != NULL)
	{	
		buffTemp = buff;
		char *pstr = strtok(buff, "\n");
		//cout<<pstr<<endl;
		
		char *qstr = NULL;  
		
		if(pstr != NULL){
			qstr = strtok(pstr, ",");  
		}else{
			continue;
		}
				
		numLines = 0;
		ifImgID = false;
		ifImgLabel = false;
		ifSingleTagImg = true;
		while(qstr != NULL)
		{
			numLines++;
			if(numLines == 1){
				//cout<<qstr<<endl;
				imgID = qstr;
				ifImgID = true;
			}else if(numLines == 2){
				//cout<<qstr<<endl;
				label = atoi(qstr);
				ifImgLabel = true;
			}else if(numLines > 2){
				ifSingleTagImg = false;
				break;
			}			
			qstr = strtok(NULL, ",");  		
		}		

		if(ifSingleTagImg && ifImgID && ifImgLabel)
		{
			singleTagDataList.imgIDList.push_back(imgID);
			singleTagDataList.label.push_back(label);
		}else if(!ifSingleTagImg){
			fprintf(fpOutputList, "%s", buffTemp.c_str());
		}		
	}

    fclose(fpQueryList);	

	return 0;
}

//
int outputMatchList(
	const string										labelTwoList)
{	
	if(singleTagDataList.imgIDList.size() != singleTagDataList.label.size()){
		cout<<"image information error!"<<endl;
		return -1;
	}

	FILE* fpDataList = fopen(labelTwoList.c_str(), "r");
	if(NULL == fpDataList)
	{
		cout<<"cannot open "<<labelTwoList<<endl;
		return -1;
	}

	int i;	
	int nRet = 0;
	char buff[2000];
	
	int numLines;
	string imgID;
	int label;
	bool ifImgID, ifImgLabel;
	int index;

	while(fgets(buff, 2000 ,fpDataList) != NULL)
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
		ifImgID = false;
		ifImgLabel = false;
		while(qstr != NULL)
		{
			numLines++;
			if(numLines == 1){				
				//cout<<qstr<<endl;
				nRet = getStringIDFromFilePath(qstr, imgID);
				if(nRet != 0){
					cout<<"image ID parse error!"<<endl;
					continue;
				}
				ifImgID = true;
			}else if(numLines == 2){
				//cout<<qstr<<endl;
				label = atoi(qstr);
				ifImgLabel = true;
			}			
			qstr = strtok(NULL, ",");		
		}		

		if(ifImgID && ifImgLabel)
		{			
			vector<string>::iterator iElement = find(singleTagDataList.imgIDList.begin(), singleTagDataList.imgIDList.end(), imgID);
			index = distance(singleTagDataList.imgIDList.begin(), iElement); 
			if(index > -1 && index < singleTagDataList.imgIDList.size()){
				fprintf(fpOutputList, "%s,%d,%d\n", singleTagDataList.imgIDList[index].c_str(), label, singleTagDataList.label[index]);
			}
		}
	}	

	fclose(fpDataList);
	return 0;
}

//
int main(int argc, char** argv) {

	const int num_required_args = 3;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts features of an image and predict its label and score.\n"
	    "Usage: Demo_mainboby szQueryList outputList\n";
	    return 1;
  }	
	
	/***********************************Init**********************************/
	string labelOneList = argv[1];		
	string labelTwoList = argv[2];	
	string outputList = argv[3];
	
	int nRet = 0;		
	/**************************** getAllDataQueryList *************************/		
	fpOutputList = fopen(outputList.c_str(), "w");
	if(NULL == fpOutputList)
	{
		cout<<"cannot open "<<outputList<<endl;
		return -1;
	}

	nRet = getSingleTagDataList(labelOneList);
	if(nRet != 0){
		cout<<"fail to getQueryList!"<<endl;
		return -1;
	}

	nRet = outputMatchList(labelTwoList);
	if(nRet != 0){
		cout<<"fail to output match patch list!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	fclose(fpOutputList);	
	
	return 0;
}

