mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
labelOneList='../data/outputList_170103_patchPropertyDelete.txt'
labelTwoList='../data/multiTag170103/propertyClass_SingleTag_170103.txt'
outputList='../data/multiTag170103/outputList_SingleTagCombine_170103.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_tagCombine $labelOneList $labelTwoList $outputList
