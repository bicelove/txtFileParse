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


struct imgMultiLabelList{
	vector<string> imgIDList;
	vector<vector<int>>	labels;
};

struct imgQueryList{
	vector<string> srcID;
	vector<string> similarID;
};

struct imgQueryList queryList;
struct imgMultiLabelList multiImg;

//
int getStringIDFromFilePath(
	const string							imageString,
	string									&imageID,
	int										&label)
{
	string imageIDTemp;
	int sour_pos, postfix_pos;
	string sour_name, nameTemp, labelName;

	int strLen = imageString.length();
	//cout<<"imageString: "<<imageString<<",\t length = "<<strLen<<endl;
	
	//
	sour_pos  = imageString.find_last_of('/');
	sour_name = imageString.substr(sour_pos + 1);
	nameTemp = sour_name;
	//cout<<"nameTemp: "<<nameTemp<<endl;
	
	postfix_pos = sour_name.find_last_of('.');
	imageIDTemp = sour_name.erase(postfix_pos, strLen - postfix_pos);  
	
	labelName = nameTemp.substr(postfix_pos + 4);
	
//	cout<<"labelName: "<<labelName<<endl;
	label = atoi(labelName.c_str());

	/*
	sour_pos = imageIDTemp.find_last_of('_');
	imageID  = imageIDTemp.substr(sour_pos + 1);
	*/
	imageID  = imageIDTemp;
	//cout<<"imageID: "<<imageID<<",\tlabel: "<<label<<endl;
	
	return 0;
}

//
int matchList(
	const string 									allDataList,
	const string									newMutiTagList)
{
	cout<<"------------------matchList------------------"<<endl;
	
	int nRet = 0;
	int i;
	char buff[1000];
	string imageID;
	int label;
	int multiLabel;
	
	int index;
	string srcImgID;	

	FILE* fpDataList = fopen(allDataList.c_str(), "r");
    if(NULL == fpDataList){
    	cout<<"cannot open "<<allDataList<<endl;
        return -1;
    }

	FILE* fpoutputList = fopen(newMutiTagList.c_str(), "w");
    if(NULL == fpoutputList){
    	cout<<"cannot open "<<newMutiTagList<<endl;
        return -1;
    }

	FILE* fpoutputData = fopen("srcJDImg_161221.txt", "w");
    if(NULL == fpoutputData){
    	cout<<"cannot open srcJDImg_161221.txt"<<endl;
        return -1;
    }
	
	while(fgets(buff, 1000, fpDataList) != NULL)
	{
		char *pstr = strtok(buff, "\n");
		
		if(pstr != NULL){
			nRet = getStringIDFromFilePath(pstr, imageID, label);
			if(nRet != 0){
				continue;
			}
			//
			vector<string>::iterator iElement = find( queryList.similarID.begin(), queryList.similarID.end(), imageID);
			index = distance(queryList.similarID.begin(), iElement); 			
			if(index > -1 && index < 7328){				
				srcImgID = queryList.srcID[index];
				//cout<<"srcImgID: "<<srcImgID<<endl;
			}else{
				continue;
			}

			fprintf(fpoutputData,"%s.jpg %d\n",srcImgID.c_str(), label);
			
			//			
			iElement = find(multiImg.imgIDList.begin(), multiImg.imgIDList.end(), srcImgID);
			index = distance(multiImg.imgIDList.begin(), iElement); 			
			if(index > -1 && index < 2722){	
				//cout<<", label:"<<label<<endl;
				fprintf(fpoutputList, "%s.jpg", multiImg.imgIDList[index].c_str());
				for(i = 0; i < multiImg.labels[index].size(); i++){
					multiLabel = multiImg.labels[index][i];
					multiLabel = multiLabel % 800;
					fprintf(fpoutputList, ",%d", multiLabel);
				}				
				fprintf(fpoutputList, ",%d\n",label + 15);
			}else{				
				fprintf(fpoutputList, "%s.jpg,%d\n", srcImgID.c_str(),label + 15);
			}
		}
	}
	
    fclose(fpDataList);
	fclose(fpoutputList);	
	fclose(fpoutputData);

	return 0;
}

//
int getMapQueryList(
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
				queryList.similarID.push_back(qstr);
			}

			qstr = strtok(NULL, ",");  
		}		
	}
	
    fclose(fpQueryList);	

	return 0;
}

//
int getMultiQueryList(
	const string 									multiTagList)
{
	cout<<"------------------getQueryList------------------"<<endl;
	
	FILE* fpQueryList = fopen(multiTagList.c_str(), "r");
    if(NULL == fpQueryList)
    {
    	cout<<"cannot open "<<multiTagList<<endl;
        return -1;
    }

	char buff[1000];
	int numLines;
	vector<int> labels;
	
	multiImg.imgIDList.clear();
	multiImg.labels.clear();
	
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
		labels.clear();
		while(qstr != NULL)
		{
			numLines++;
			if(numLines == 1){
				string imageIDTemp = qstr;
				string newID;
				int postfix_pos = imageIDTemp.find_last_of('.');
				newID = imageIDTemp.erase(postfix_pos, 4);  
				multiImg.imgIDList.push_back(newID);
			}else if(numLines >= 2){
				//cout<<qstr<<endl;		
				labels.push_back(atoi(qstr));				
			}
			qstr = strtok(NULL, ",");  
		}		
		multiImg.labels.push_back(labels);
	}
	
    fclose(fpQueryList);	

	return 0;
}


//
int main(int argc, char** argv) {

	const int num_required_args = 5;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts features of an image and predict its label and score.\n"
	    "Usage: Demo_mainboby multiTagList allDataList mapIDList newMutiTagList\n";
	    return 1;
  }	
	
	/***********************************Init**********************************/
	string multiTagList = argv[1];	
	string allDataList = argv[2];		
	string mapIDList = argv[3];
	string newMutiTagList = argv[4];
	
	int nRet;	

	nRet = 0;	
	/**************************** imagePredict Deal *************************/
	nRet = getMultiQueryList(multiTagList);
	if(nRet != 0){
		cout<<"fail to getQueryList!"<<endl;
		return -1;
	}
	
	nRet = getMapQueryList(mapIDList);
	if(nRet != 0){
		cout<<"fail to getMapQueryList!"<<endl;
		return -1;
	}

	nRet = matchList(allDataList, newMutiTagList);
	if(nRet != 0){
		cout<<"fail to matchList!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	return 0;
}

