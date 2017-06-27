mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='../data/outputList_170103.txt'
IDList='../data/170103_classData.txt'
outputList='../data/outputList_170103_patchPropertyDelete.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_patchPropertyDelete $szQueryList $IDList $outputList
