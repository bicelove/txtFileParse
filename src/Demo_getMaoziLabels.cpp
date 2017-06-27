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
struct imgDataQueryList{
	vector<string>	imgIDList;
	vector<vector<int>>	labels;
};
struct imgDataQueryList dataQueryList;

//
int getStringID(
	const string							imageString,
	string 									&srcImgID)
{
	int sour_pos;
	
	//
	sour_pos = imageString.find_last_of('/');
	srcImgID = imageString.substr(sour_pos + 1);

	return 0;
}

//
int getDataList(
	const string 									szQueryList)
{
	cout<<"------------------getSingleTagDataList------------------"<<endl;
	
	FILE* fpQueryList = fopen(szQueryList.c_str(), "r");
    if(NULL == fpQueryList)
    {
    	cout<<"cannot open "<<szQueryList<<endl;
        return -1;
    }	

	int nRet;
	char buff[2000];	
	int numLines;	
	string imgID;
	int label;
	bool ifImgID, ifImgLabel;
	vector<int> labels;
	
	dataQueryList.imgIDList.clear();
	dataQueryList.labels.clear();
	while(fgets(buff, 2000, fpQueryList) != NULL)
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
		labels.clear();
		while(qstr != NULL)
		{
			numLines++;
			if(numLines == 1){
				//cout<<qstr<<endl;
				nRet = getStringID(qstr, imgID);
				if(nRet != 0)break;
				ifImgID = true;
			}else if(numLines >= 2){
				//cout<<qstr<<endl;
				label = atoi(qstr);
				labels.push_back(label);
				ifImgLabel = true;
			}	
			qstr = strtok(NULL, ",");  		
		}		

		if(ifImgID && ifImgLabel)
		{				
			dataQueryList.imgIDList.push_back(imgID);
			dataQueryList.labels.push_back(labels);
		}	
	}

    fclose(fpQueryList);	

	return 0;
}

//
int outputMatchList(
	const string										srcDataList,
	const string										outputDataList)
{	
	if(dataQueryList.imgIDList.size() < 1 || dataQueryList.labels.size() < 1 
		|| dataQueryList.imgIDList.size() != dataQueryList.labels.size()){
		cout<<"list information error!"<<endl;
		return -1;
	}
	
	FILE* fpSrcData = fopen(srcDataList.c_str(), "r");
	if(NULL == fpSrcData)
	{
		cout<<"cannot open "<<srcDataList<<endl;
		return -1;
	}

	FILE* fpOutputList = fopen(outputDataList.c_str(), "w");
	if(NULL == fpOutputList)
	{
		cout<<"cannot open "<<outputDataList<<endl;
		return -1;
	}

	/********************************************************/
	int i;	
	int nRet;
	char buff[2000];
	string imgID;
	string buffTemp;
	int index;
	
	while(fgets(buff, 2000, fpSrcData) != NULL)
	{
		buffTemp = buff;
		char *pstr = strtok(buff, "\n");
		//cout<<pstr<<endl;

		nRet = getStringID(pstr, imgID);
		if(nRet != 0)continue;

		cout<<"imgID = "<<imgID<<endl;
		vector<string>::iterator iElement = find(dataQueryList.imgIDList.begin(), dataQueryList.imgIDList.end(), imgID);
		index = distance(dataQueryList.imgIDList.begin(), iElement); 
		cout<<"index = "<<index<<endl;
		if(index > -1 && index < dataQueryList.imgIDList.size()){				
			fprintf(fpOutputList, "%s", pstr);
			for(i = 0; i < dataQueryList.labels[index].size(); i++){						
				fprintf(fpOutputList, ",%d", dataQueryList.labels[index][i]);
			}
			fprintf(fpOutputList, "\n"); 
		}			
	}
	
	fclose(fpSrcData);	
	fclose(fpOutputList);
	
	return 0;
}

//
int main(int argc, char** argv) {

	const int num_required_args = 4;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts features of an image and predict its label and score.\n"
	    "Usage: Demo_getChenshanData szQueryList srcDataList outputDataList\n";
	    return 1;
  }	
	
	/***********************************Init**********************************/
	string szQueryList = argv[1];	
	string srcDataList = argv[2];
	string outputDataList = argv[3];
	
	int nRet = 0;		
	/**************************** getAllDataQueryList *************************/
	nRet = getDataList(szQueryList);
	if(nRet != 0){
		cout<<"fail to getSingleTagDataList!"<<endl;
		return -1;
	}

	//
	nRet = outputMatchList(srcDataList, outputDataList);
	if(nRet != 0){
		cout<<"fail to output match patch list!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	return 0;
}
