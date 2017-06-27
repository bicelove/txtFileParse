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

#define numClass 307
struct imgMultiQueryList{
	vector<string> imgIDList;
	vector<vector<int>>	labels;
};

struct imgMultiQueryList multiList;

//
int GetMultiList(
	const string										IDList)
{
	FILE* fpDataList = fopen(IDList.c_str(), "r");
    if(NULL == fpDataList){
    	cout<<"cannot open "<<IDList<<endl;
        return -1;
    }
	
	int i;	
	int nRet = 0;
	char buff[1000];
	string imgID;
	int numLines;
	int label;
	bool ifImgID, ifImgLabel;
	vector<int> labels;
	
	multiList.imgIDList.clear();
	multiList.labels.clear();
	
	while(fgets(buff, 1000, fpDataList) != NULL)
	{		
		char *pstr = strtok(buff, "\n");	
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
				imgID = qstr;
				ifImgID = true;
			}else if(numLines >= 2){
				//cout<<qstr<<endl;
				label = atoi(qstr);
				if(label < 0 || label >= numClass)continue;
				labels.push_back(label);
				ifImgLabel = true;
			}
			
			qstr = strtok(NULL, ",");  		
		}		

		if(ifImgID && ifImgLabel)
		{
			multiList.imgIDList.push_back(imgID);
			multiList.labels.push_back(labels);
		}		

	}
	
	fclose(fpDataList);

	return 0;
}

//
int outputNumsEachLabel(
	const string										outputList)
{
	if(multiList.imgIDList.size() != multiList.labels.size()){
		cout<<"multi List error!"<<endl;
		cout<<"multiList.imgIDList.size = "<<multiList.imgIDList.size()<<"; ";
		cout<<"multiList.labels.size = "<<multiList.labels.size()<<endl;
		return -1;
	}
	
	FILE* fpOutputList = fopen(outputList.c_str(), "w");
	if(NULL == fpOutputList){
		cout<<"cannot open "<<outputList<<endl;
		return -1;
	}

	int i, j;	
	int nRet = 0;
	vector<int> numEachLabel;
	int index;

	numEachLabel.resize(numClass, 0);
	for(i = 0; i < multiList.imgIDList.size(); i++){	
		for(j = 0; j < multiList.labels[i].size(); j++){			
			index = multiList.labels[i][j];
			if(index > -1 && index < numClass){
				numEachLabel[index]++;
			}
		}		
	}
	for(i = 0; i < numClass; i++){
		fprintf(fpOutputList, "%d:	%d\n", i, numEachLabel[i]);
	}

	fclose(fpOutputList);	

	return 0;
}

//
int main(int argc, char** argv) {

	const int num_required_args = 3;
	if( argc < num_required_args ){
	    cout<<
	    "This program counts the numbers of each label.\n"
	    "Usage: Demo_numsEachLabel szQueryList outputList\n";
	    return 1;
  }		
	
	/***********************************Init**********************************/
	string szQueryList = argv[1];
	string outputList = argv[2];
	
	int nRet;	
	nRet = 0;	
	/**************************** getAllDataQueryList *************************/
	nRet = GetMultiList(szQueryList);
	if(nRet != 0){
		cout<<"fail to match patch list!"<<endl;
		return -1;
	}
	
	nRet = outputNumsEachLabel(outputList);
	if(nRet != 0){
		cout<<"fail to output match patch list!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	return 0;
}
