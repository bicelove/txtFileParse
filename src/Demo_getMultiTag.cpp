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
int propertyNum = 13;
int classNum = 29;
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
	string									&imageID,
	int										&label)
{
	int sour_pos, postfix_pos;
	string sour_name, nameTemp1, labelName;

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
		imageID = srcImgID.substr(0, postfix_pos - 1);
	}	
	//cout<<"imageID: "<<imageID<<endl;
	
	labelName = sour_name.substr(sour_pos + 5);
	label = atoi(labelName.c_str());
	//cout<<"label: "<<label<<endl;
	
	return 0;

}

//
int getStringIDFromFilePath(
	const string							imageString,
	string 									&srcImgID,
	string									&imageID,
	int										&label)
{
	string imageIDTemp;
	int sour_pos, postfix_pos;
	string sour_name, nameTemp1, nameTemp2, nameTemp3, labelName;

	//cout<<"imageString: "<<imageString<<",\t length = "<<strLen<<endl;
	
	//
	sour_pos  = imageString.find_last_of('/');
	sour_name = imageString.substr(sour_pos + 1);
	nameTemp1 = sour_name;
	//cout<<"nameTemp1: "<<nameTemp1<<endl;

	//
	sour_pos  = sour_name.find_last_of('.');	
	srcImgID = nameTemp1.erase(sour_pos);
	
	postfix_pos = sour_name.find_last_of('_');
	nameTemp2 = sour_name.substr(postfix_pos + 1);
	nameTemp3 = nameTemp2;

	int strLen = nameTemp2.length();
	postfix_pos = nameTemp2.find_last_of('.');
	imageIDTemp = nameTemp2.erase(postfix_pos, strLen - postfix_pos);  
	
	labelName = nameTemp3.substr(postfix_pos + 4);
	
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
	//cout<<"sour_pos = "<<sour_pos<<endl;	
	sour_name	= imageString.substr(sour_pos + 1);
	postfix_pos = sour_name.find_last_of('.');
	imageIDTemp = sour_name.erase(postfix_pos, 4);  		

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

	//
	sour_pos = imageIDTemp.find_last_of('_');
	imageID  = imageIDTemp.substr(sour_pos + 1);

	return 0;
}


//
int imgIDMap(
	const string 									similarIDList,
	const string 									srcIDList,
	const string									outputList)
{
	cout<<"------------------imgIDMap------------------"<<endl;

	FILE* fpSimilarDataList = fopen(similarIDList.c_str(), "r");
    if(NULL == fpSimilarDataList){
    	cout<<"cannot open "<<similarIDList<<endl;
        return -1;
    }

	FILE* fpSrcDataList = fopen(srcIDList.c_str(), "r");
    if(NULL == fpSrcDataList){
    	cout<<"cannot open "<<srcIDList<<endl;
        return -1;
    }

	FILE* fpOutputList = fopen(outputList.c_str(), "w");
    if(NULL == fpOutputList)
    {
    	cout<<"cannot open "<<outputList<<endl;
        return -1;
    }

	int nRet = 0;
	int i;
	char buff[1000];
	string imgIDTemp;
	int index;

	int numLines;
	struct srcImgIDList srcList;
	srcList.imgIDs.clear();
	srcList.keys.clear();

	while(fgets(buff, 1000, fpSrcDataList) != NULL)
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
			//cout<<qstr<<endl;
			numLines++;
			if(numLines == 1){
				srcList.imgIDs.push_back(qstr);
			}else if(numLines == 2){		
				//cout<<"key = "<<qstr<<endl;
				srcList.keys.push_back(qstr);
			}
			
			qstr = strtok(NULL, ",");  
		}
	}

	//
	while(fgets(buff, 1000, fpSimilarDataList) != NULL)
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
				imgIDTemp = qstr;
				
			}else if(numLines == 2){		
				//cout<<"numLabels = "<<numLabel<<endl;
				vector<string>::iterator iElement = find(srcList.keys.begin(), srcList.keys.end(), qstr);
				index = distance(srcList.keys.begin(), iElement); 
				if(index > -1){							
					fprintf(fpOutputList, "%s,%s\n", srcList.imgIDs[index].c_str(), imgIDTemp.c_str());
				}						
			}
			
			qstr = strtok(NULL, ",");  
		}
	}
	
	fclose(fpOutputList);
    fclose(fpSrcDataList);
    fclose(fpSimilarDataList);

	return 0;
}

//
int matchSrcList(
	const string 									srcIDList,
	const string									outputList)
{
	cout<<"------------------matchList------------------"<<endl;

	int nRet = 0;
	int i;
	char buff[1000];
	string srcImgID, imageID;
	string rectLabel;
	vector<int> indexRecord;
	int index;
	int imgLabel, label;
	vector<int> labels;	
	vector<int> labelNum(classNum ,0);
	int numIMg;

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
	FILE* fpDetectList = fopen("Dict_Image_Cloth_Predict.txt", "r");
    if(NULL == fpDetectList){
    	cout<<"cannot open Dict_Image_Cloth_Predict.txt"<<endl;
        return -1;
    }
	
	FILE* fpOutClassList = fopen("classNum.txt", "w");
	if(NULL == fpOutClassList)
	{
		cout<<"cannot open classNum.txt"<<endl;
		return -1;
	}

	indexRecord.clear();
	numIMg = 0;
	//cout<<"queryList.size = "<<queryList.imgIDList.size()<<endl;
	while(fgets(buff, 1000, fpDataList) != NULL)
	{
		char *pstr = strtok(buff, "\n");
		nRet = getStringID(pstr, srcImgID, imageID, imgLabel);
		if(nRet != 0){
			continue;
		}
		/*
		cout<<"----------------------------------"<<endl;
		cout<<"imageID :"<<imageID<<endl;
		*/
		//cout<<"imgLabel :"<<imgLabel<<endl;
				
		if(pstr != NULL){
		
			vector<string>::iterator iElement = find( queryList.imgIDList.begin(), queryList.imgIDList.end(), imageID);
			index = distance(queryList.imgIDList.begin(), iElement); 
		
			fprintf(fpOutputList, "%s.jpg,", srcImgID.c_str());
			if(index > -1 && index <  queryList.imgIDList.size() && queryList.labels[index].size() == queryList.numLable[index]){
				//cout<<"index = "<<index<<endl;	
				//cout<<"num of labels = "<<queryList.labels[index].size()<<endl;
				//cout<<"numLabels = "<<queryList.numLable[index]<<endl;
							
				indexRecord.push_back(index);			
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
				//if(labels.size() < 1)continue;
				
				for(i = 0; i < labels.size(); i++){
					label = labels[i] % 800;
					if(label == 9 || label == 10){
						continue;
					}else if(label >= 11 && label <= 15){
						label -= 2;
					}
					fprintf(fpOutputList, "%d,", label);	
					labelNum[label]++;
					//cout<<label<<" ";	
				}			
				//cout<<endl;			
			}
			fprintf(fpOutputList, "%d\n", imgLabel + propertyNum);
			labelNum[imgLabel + propertyNum]++;
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
int matchList(
	const string 									similarIDList,
	const string 									srcIDList,
	const string									outputList)
{
	cout<<"------------------matchList------------------"<<endl;

	int nRet = 0;
	int i;
	char buff[1000];
	string imageID;
	string rectLabel;
	vector<int> indexRecord;
	int index;
	int label;
	vector<int> labels;
	
	string imgIDMapTemp = "imgIDMapTemp.txt";
		
	nRet = imgIDMap(similarIDList, srcIDList, imgIDMapTemp);
	if(nRet != 0){
		cout<<"map image ID error!"<<endl;
		return -1;
	}	

	FILE* fpDataList = fopen(imgIDMapTemp.c_str(), "r");
    if(NULL == fpDataList){
    	cout<<"cannot open "<<imgIDMapTemp<<endl;
        return -1;
    }

	FILE* fpOutputList = fopen(outputList.c_str(), "w");
    if(NULL == fpOutputList)
    {
    	cout<<"cannot open "<<outputList<<endl;
        return -1;
    }

	indexRecord.clear();
	while(fgets(buff, 1000, fpDataList) != NULL)
	{
		char *pstr = strtok(buff, "\n");
		char *qstr = strtok(pstr, ",");
		
		if(qstr != NULL){
			//cout<<qstr<<endl;
			/*
			nRet = getStringIDFromFilePath(qstr, imageID, rectLabel);
			if(nRet != 0){
				continue;
			}
			cout<<"imageID = "<<imageID<<endl;
			*/
			vector<string>::iterator iElement = find( queryList.imgIDList.begin(), queryList.imgIDList.end(), qstr);
			index = distance(queryList.imgIDList.begin(), iElement); 

			
			if(index > -1 && index < queryList.imgIDList.size() && queryList.labels[index].size() == queryList.numLable[index]){
				//cout<<"index = "<<index<<endl;	
				//cout<<"num of labels = "<<queryList.labels[index].size()<<endl;
				//cout<<"numLabels = "<<queryList.numLable[index]<<endl;
							
				indexRecord.push_back(index);			
				labels.clear();
				for(i = 0; i < queryList.numLable[index]; i++){
					label = queryList.labels[index][i];
					if(label / 800 == 1 && label % 800 < 100){
						labels.push_back(label);
					}
				}	

				if(labels.size() < 1)continue;
				
				fprintf(fpOutputList, "%s.jpg", qstr);
				//cout<<index<<": ";
				for(i = 0; i < labels.size(); i++){
					label = labels[i];
					fprintf(fpOutputList, ",%d", label);
				}				
				//cout<<endl;
				fprintf(fpOutputList, "\n");
			}			
		}
	}
	
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

	char buff[2000];
	int numLines, numPlabels;
	int	labelIndex;
	
	string imgID;
	int numLabel;
	vector<int> label;

	bool ifImgID, ifnumLabel, ifNumLines;
	
	queryList.imgIDList.clear();
	queryList.labels.clear();
	queryList.numLable.clear();
	
	while(fgets(buff, 2000 ,fpQueryList) != NULL)
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

	const int num_required_args = 5;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts features of an image and predict its label and score.\n"
	    "Usage: Demo_mainboby szQueryList similarIDList srcIDList outputList\n";
	    return 1;
  }	
	
	/***********************************Init**********************************/
	string szQueryList = argv[1];	
	string similarIDList = argv[2];	
	string srcIDList = argv[3];
	string outputList = argv[4];
	
	int nRet;	

	nRet = 0;	
	/**************************** imagePredict Deal *************************/
	nRet = getQueryList(szQueryList);
	if(nRet != 0){
		cout<<"fail to getQueryList!"<<endl;
		return -1;
	}
	/*
	nRet = matchList(similarIDList, srcIDList, outputList);
	if(nRet != 0){
		cout<<"fail to matchList!"<<endl;
		return -1;
	}
	*/
	nRet = matchSrcList(srcIDList, outputList);
	if(nRet != 0){
		cout<<"fail to matchList!"<<endl;
		return -1;
	}
	
	cout<<"deal end!"<<endl;
	
	return 0;
}
