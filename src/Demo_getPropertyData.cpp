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
	vector<string> imgIDList;
	vector<int>	numLable;
	vector<vector<int>>	labels;
};

struct srcImgIDList{
	vector<string> imgIDs;
	vector<string> keys;
};

struct imgQueryList queryList;
int propertyNum = 15;

//
int getLabelIndex(
	const string							imageString)
{
	int sour_pos;
	string sour_index;
	int	labelIndex;

	//
	sour_pos   = imageString.find_first_of('_');	
	sour_index = imageString.substr(0, sour_pos);
	
	labelIndex = atoi(sour_index.c_str());

	return labelIndex;
}

//
int getStringID(
	const string							imageString,
	string 									&srcImgID,
	string									&imageID)
{
	int sour_pos, postfix_pos;
	string sour_name, nameTemp1;

	//cout<<"imageString: "<<imageString<<",\t length = "<<strLen<<endl;
	
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
		imageID = srcImgID.substr(postfix_pos + 1);
	}	
	//cout<<"imageID: "<<imageID<<endl;
	
	return 0;
}

//
int matchSrcList(
	const string 									srcIDList,
	const string									outputList,
	const string									svPath)
{
	cout<<"------------------matchList------------------"<<endl;

	int nRet = 0;
	int i;
	char buff[2000];
	string srcImgID, imageID;
	string imgPath;
	
	int index;
	int label;
	vector<int> labels;	
	vector<int> labelNum(propertyNum ,0);
	int numIMg;
	int minIndex;
	
	/* ********************************************** */
	FILE* fpDataList = fopen(srcIDList.c_str(), "r");
    if(NULL == fpDataList){
    	cout<<"cannot open "<<srcIDList<<endl;
        return -1;
    }

	FILE* fpOutputList = fopen(outputList.c_str(), "w");
    if(NULL == fpOutputList)
    {
    	cout<<"cannot open "<<outputList<<endl;
        return -1;
    }
	/* ********************************************** */
	FILE* fpDetectList = fopen("Dict_Image_Cloth_Property.txt", "r");
    if(NULL == fpDetectList){
    	cout<<"cannot open Dict_Image_Cloth_Property.txt"<<endl;
        return -1;
    }
	
	FILE* fpOutClassList = fopen("propertyNum.txt", "w");
	if(NULL == fpOutClassList)
	{
		cout<<"cannot open propertyNum.txt"<<endl;
		return -1;
	}
	/* ********************************************** */

	numIMg = 0;
	//cout<<"queryList.size = "<<queryList.imgIDList.size()<<endl;
	while(fgets(buff, 2000, fpDataList) != NULL)
	{
		char *pstr = strtok(buff, "\n");
		imgPath = pstr;
		nRet = getStringID(pstr, srcImgID, imageID);
		if(nRet != 0){
			cout<<"getStringID error!"<<endl;
			continue;
		}
		/*
		cout<<"----------------------------------"<<endl;
		cout<<"imageID :"<<imageID<<endl;
		*/
		//cout<<"imgLabel :"<<imgLabel<<endl;
				
		if(pstr != NULL){		
			vector<string>::iterator iElement = find(queryList.imgIDList.begin(), queryList.imgIDList.end(), imageID);
			index = distance(queryList.imgIDList.begin(), iElement); 
		
			if(index > -1 && index < queryList.imgIDList.size() && queryList.labels[index].size() == queryList.numLable[index]){
				labels.clear();
				//cout<<"labels :";
				for(i = 0; i < queryList.numLable[index]; i++){
					label = queryList.labels[index][i];
					if(label / 800 == 1 && label % 800 < 100){
						labels.push_back(label);
						//cout<<label<<" ";
					}
				}	
				//cout<<endl;
				if(labels.size() < 1)continue;			

				//output		
				for(i = 0; i < labels.size(); i++){
					label = labels[i] % 800;
					if(labelNum[label] >= 5000)continue; 
					if(label == 9 || label == 10){
						continue;
					}else if(label >= 11 && label <= 15){
						label -= 2;
					}
					fprintf(fpOutputList, "%s ", pstr);
					fprintf(fpOutputList, "%d\n", label);	
					labelNum[label]++;	

					//save image
					std::stringstream labelTemp;
		            labelTemp << (label);
					std::string dist_path = svPath + "/" + labelTemp.str() + "/" + srcImgID + ".jpg";
			        std::string final_path = "cp "+ imgPath + " " + dist_path;
			        system(final_path.c_str());
				}
				
				/*
				minIndex = *min_element(labels.begin(), labels.end()); 
				label = minIndex % 800;
				if(labelNum[label] >= 5000)continue; 
				if(label == 9 || label == 10){
					continue;
				}else if(label >= 11 && label <= 15){
					label -= 2;
				}
				fprintf(fpOutputList, "%s ", pstr);
				fprintf(fpOutputList, "%d\n", label);	
				labelNum[label]++;	
				*/
			}		
		}
		numIMg++;
		if(numIMg % 100 ==0)
			cout<<"the "<<numIMg<<"th image!"<<endl;
	}
	
	int labelNumAll = 0;
	for(i = 0; i < labelNum.size(); i++){
		labelNumAll += labelNum[i];
		char *pstr = NULL;
		if(fgets(buff, 1000, fpDetectList) != NULL){
			pstr = strtok(buff, " \t\n");
		}
		fprintf(fpOutClassList, "%s:	%d\n", pstr, labelNum[i]);
	}

	cout<<"label num per image: "<<labelNumAll<<"/"<<numIMg<<" = "<<labelNumAll*1.0/numIMg<<endl;

	fclose(fpDetectList);
	fclose(fpOutClassList);
	fclose(fpOutputList);
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

	char buff[50000];
	int numLines, numPlabels;
	int	labelIndex;
	
	string imgID;
	int numLabel;
	vector<int> label;

	bool ifImgID, ifnumLabel, ifNumLines;
	
	queryList.imgIDList.clear();
	queryList.labels.clear();
	queryList.numLable.clear();
	
	while(fgets(buff, 50000 ,fpQueryList) != NULL)
	{	
		char *pstr = strtok(buff, "\n");
		//cout<<pstr<<endl;
		
		char *qstr = NULL;  
		char *str = NULL;
		
		if(pstr != NULL){
			qstr = strtok(pstr, ";");  
		}else{
			continue;
		}
				
		numLines = 0;
		ifImgID = false;
		ifnumLabel = false;
		ifNumLines = false;
		while(qstr != NULL)
		{
			numLines++;
			if(numLines == 1){
				imgID = qstr;
				ifImgID = true;
			}else if(numLines == 2){		
				//cout<<"numLabels str = "<<qstr<<endl;
				numLabel = atoi(qstr);
				//cout<<"numLabels = "<<numLabel<<endl;
				ifnumLabel = true;
			}else if(numLines == 3){
				//cout<<qstr<<endl;
				str =  strtok(qstr, ","); 
				numPlabels = 0;
				label.clear();
				while(str != NULL)
				{	
					numPlabels++;
					if(numPlabels > numLabel)
						break;
					labelIndex = getLabelIndex(str);
					label.push_back(labelIndex);
					str = strtok(NULL, ",");  
				}
				if(numLabel == label.size())ifNumLines = true;
			}

			str = qstr + strlen(qstr) + 1;
			qstr = strtok(str, ";");  
		}		
		
		if(ifImgID && ifnumLabel && ifNumLines)
		{
			queryList.imgIDList.push_back(imgID);
			queryList.numLable.push_back(numLabel);
			queryList.labels.push_back(label);
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
	    "Usage: Demo_mainboby szQueryList imgList outputList\n";
	    return 1;
  }	
	
	/***********************************Init**********************************/
	string szQueryList = argv[1];	
	string imgList = argv[2];
	string outputList = argv[3];
	string svPath = argv[4];
	
	int nRet;	

	nRet = 0;	
	/**************************** imagePredict Deal *************************/
	nRet = getQueryList(szQueryList);
	if(nRet != 0){
		cout<<"fail to getQueryList!"<<endl;
		return -1;
	}
	cout<<"queryList.size = "<<queryList.imgIDList.size()<<endl;
	
	nRet = matchSrcList(imgList, outputList, svPath);
	if(nRet != 0){
		cout<<"fail to matchList!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	return 0;
}
