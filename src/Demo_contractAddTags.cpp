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

vector<string>	srcTagsQueryList;

//
int getQueryList(
	const string 									szQueryList)
{
	cout<<"------------------getSingleTagDataList------------------"<<endl;
	char buff[2000];
	
	FILE* fpQueryList = fopen(szQueryList.c_str(), "r");
    if(NULL == fpQueryList){
    	cout<<"cannot open "<<szQueryList<<endl;
        return -1;
    }	

	srcTagsQueryList.clear();
	while(fgets(buff, 2000, fpQueryList) != NULL){	
		char *pstr = strtok(buff, "\n");
		//cout<<pstr<<endl;
		
		srcTagsQueryList.push_back(pstr);		
	}

    fclose(fpQueryList);	

	return 0;
}

//
int outputMatchList(
	const string										newTagsList,
	const string										outputOldTagsList,
	const string										outputAddTagsList)
{	
	if(srcTagsQueryList.size() < 1){
		cout<<"list information error!"<<endl;
		return -1;
	}
	
	FILE* fpNewTagsData = fopen(newTagsList.c_str(), "r");
	if(NULL == fpNewTagsData)
	{
		cout<<"cannot open "<<newTagsList<<endl;
		return -1;
	}

	FILE* fpOutputOldList = fopen(outputOldTagsList.c_str(), "w");
	if(NULL == fpOutputOldList)
	{
		cout<<"cannot open "<<outputOldTagsList<<endl;
		return -1;
	}

	FILE* fpOutputAddList = fopen(outputAddTagsList.c_str(), "w");
	if(NULL == fpOutputAddList)
	{
		cout<<"cannot open "<<outputAddTagsList<<endl;
		return -1;
	}

	/********************************************************/
	char buff[2000];
	string buffTemp;
	int index;
	
	while(fgets(buff, 2000, fpNewTagsData) != NULL){
		char *pstr = strtok(buff, "\n");
		buffTemp = pstr;
		//cout<<pstr<<endl;		

		vector<string>::iterator iElement = find(srcTagsQueryList.begin(), srcTagsQueryList.end(), buffTemp);
		index = distance(srcTagsQueryList.begin(), iElement); 
		//cout<<"index = "<<index<<endl;
			
		if(index > -1 && index < srcTagsQueryList.size()){	
			fprintf(fpOutputOldList, "%s\n", buffTemp.c_str());			
		}else{
			fprintf(fpOutputAddList, "%s\n", buffTemp.c_str());	
		}
	}
	
	fclose(fpNewTagsData);	
	fclose(fpOutputOldList);
	fclose(fpOutputAddList);
	
	return 0;
}

//
int main(int argc, char** argv) {

	const int num_required_args = 4;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts features of an image and predict its label and score.\n"
	    "Usage: Demo_contractTags szQueryList newTagsList outputOldTagsList outputAddTagsList\n";
	    return 1;
  }	
	
	/***********************************Init**********************************/
	string szQueryList = argv[1];	
	string newTagsList = argv[2];
	string outputOldTagsList = argv[3];
	string outputAddTagsList = argv[4];
	
	int nRet = 0;		
	/**************************** getAllDataQueryList *************************/
	nRet = getQueryList(szQueryList);
	if(nRet != 0){
		cout<<"fail to getSingleTagDataList!"<<endl;
		return -1;
	}

	//
	nRet = outputMatchList(newTagsList, outputOldTagsList, outputAddTagsList);
	if(nRet != 0){
		cout<<"fail to output match patch list!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	return 0;
}
