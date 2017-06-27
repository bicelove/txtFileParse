mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='../data/outputList_170103_patchPropertyDelete.txt'
outputSingleTagList='../data/multiTag170103/outputList_singleTagData_170103.txt'
outputMultiTagList='../data/multiTag170103/outputList_MultiTagData_170103.txt'
outputMultiTagClassLabelList='../data/multiTag170103/outputList_MultiTagData_classLabel_170103.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_getSingleTagData $szQueryList $outputSingleTagList $outputMultiTagList $outputMultiTagClassLabelList
