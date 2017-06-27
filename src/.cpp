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
int getStringID(
	const string							imageString,
	string 									&srcImgID,
	string									&imageID)
{
	int sour_pos, postfix_pos;
	string sour_name, nameTemp1;
	
	//
	sour_pos  = imageString.find_last_of('/');
	sour_name = imageString.substr(sour_pos + 1);
	nameTemp1 = sour_name;
	//cout<<"nameTemp1: "<<nameTemp1<<endl;

	//
	sour_pos  = sour_name.find_last_of('.');	
	srcImgID = nameTemp1.erase(sour_pos);
	//cout<<"srcImgID: "<<srcImgID<<endl;
	
	postfix_pos = srcImgID.find_last_of('_');
	if(postfix_pos < 0){
		imageID = srcImgID;
	}else{
		imageID = srcImgID.substr(0, postfix_pos);
	}	
	//cout<<"imageID: "<<imageID<<endl;
	
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
	string imgIDPath;
	int imgLabel;
	string srcImgID, imgID;
	
	while(fgets(buff, 1000 ,fpQueryList) != NULL)	{	
		
		char *pstr = strtok(buff, "\n");
		imgIDPath = strtok(pstr, ",");
		imgLabel = atoi(strtok(NULL, ","));
		//cout<<"imgIDPath: "<<imgIDPath<<endl;
		//cout<<"imgLabel: "<<imgLabel<<endl;
		
		nRet = getStringID(imgIDPath, srcImgID, imgID);
		if(nRet != 0){
			continue;
		}
		//cout<<"srcImgID: "<<srcImgID<<endl;
		//cout<<"imgID :"<<imgID<<endl;
		if(srcImgID == imgID){
			queryList.push_back(srcImgID);
		}else{
			patchList.push_back(make_pair(srcImgID, imgLabel));
		}
	}	
	
    fclose(fpQueryList);	

	return 0;
}

//
int matchList()
{
	cout<<"------------------matchList------------------"<<endl;

	int i;
	int index;
	
	int postfix_pos;
	string imageID;
	
	for(i = 0; i < patchList.size(); i++){
			
		postfix_pos =  patchList[i].first.find_last_of('_');
		imageID = patchList[i].first.substr(0, postfix_pos);
		//cout<<"postfix_pos: "<<postfix_pos<<endl;
		
		vector<string>::iterator iElement = find( queryList.begin(), queryList.end(), imageID);
		index = distance(queryList.begin(), iElement); 
		if(index > -1 && index < queryList.size()){
			patchMatchList.push_back(patchList[i]);	
		}
	}

	return 0;
}

//
int outputMatchList(
	const string										szQueryList,
	const string										outputList)
{
	FILE* fpDataList = fopen(szQueryList.c_str(), "r");
    if(NULL == fpDataList){
    	cout<<"cannot open "<<szQueryList<<endl;
        return -1;
    }
	
	FILE* fpOutputList = fopen(outputList.c_str(), "w");
	if(NULL == fpOutputList)
	{
		cout<<"cannot open "<<outputList<<endl;
		return -1;
	}

	int i;	
	int nRet = 0;
	char buff[1000];
	string srcImgID, imgID, buffTemp;
	vector<string> imgIDTemp;
	int index;

	imgIDTemp.clear();
	for(i = 0; i < patchMatchList.size(); i++){
		imgIDTemp.push_back(patchMatchList[i].first);
	}
	if(patchMatchList.size() != imgIDTemp.size()){
		cout<<"image ID temp error!"<<endl;
		return -1;
	}
	
	while(fgets(buff, 1000, fpDataList) != NULL)
	{
		buffTemp = buff;
		
		char *pstr = strtok(buff, "\n");	
		char *qstr = NULL;  
		
		if(pstr != NULL){
			qstr = strtok(pstr, ",");  
		}else{
			continue;
		}
		
		nRet = getStringID(qstr, srcImgID, imgID);
		if(nRet != 0){
			continue;
		}
		vector<string>::iterator iElement = find(imgIDTemp.begin(), imgIDTemp.end(), srcImgID);
		index = distance(imgIDTemp.begin(), iElement); 
		if(index > -1 && index < patchMatchList.size()){
			//cout<<"id: "<<patchMatchList[index].first<<endl;
			//cout<<"label: "<<patchMatchList[index].second<<endl;
			fprintf(fpOutputList, "%s.jpg,%d\n", patchMatchList[index].first.c_str(), patchMatchList[index].second);
		}else{
			fprintf(fpOutputList, "%s", buffTemp.c_str());
		}
	}
	
	fclose(fpDataList);
	fclose(fpOutputList);	

	return 0;
}

//
int main(int argc, char** argv) {

	const int num_required_args = 4;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts features of an image and predict its label and score.\n"
	    "Usage: Demo_mainboby szQueryList IDList outputList\n";
	    return 1;
  }		
	
	/***********************************Init**********************************/
	string szQueryList = argv[1];	
	string IDList = argv[2];	
	string outputList = argv[3];
	
	int nRet;	
	char buff[1000];
	nRet = 0;	
	
	/**************************** getAllDataQueryList *************************/
	FILE* fpDataList = fopen(IDList.c_str(), "r");
    if(NULL == fpDataList){
    	cout<<"cannot open "<<IDList<<endl;
        return -1;
    }
	
	patchMatchList.clear();
	while(fgets(buff, 1000, fpDataList) != NULL)
	{
		queryList.clear();
		patchList.clear();
		char *pstr = strtok(buff, "\n");	//imgID,imgLabel
		nRet = getQueryList(pstr);
		if(nRet != 0){
			cout<<"fail to getQueryList!"<<endl;
			return -1;
		}
		
		nRet = matchList();
		if(nRet != 0){
			cout<<"fail to match patch list!"<<endl;
			return -1;
		}
	}
	fclose(fpDataList);
	
	nRet = outputMatchList(szQueryList, outputList);
	if(nRet != 0){
		cout<<"fail to output match patch list!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	return 0;
}
