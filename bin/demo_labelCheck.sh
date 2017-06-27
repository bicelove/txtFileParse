mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='../data/170103_classData.txt'
IDList='../data/multiTag170103/170103AllPositiveData.txt'
outputList='../data/outputList_170103_unmatchList.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_labelCheck $szQueryList $IDList $outputList
