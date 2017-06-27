mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
labelOneList='../data/cloth_161228_srcAndPatch.txt'
labelTwoList='../data/multiTag161228/other.txt'
outputList='../data/multiTag161228/outputList_other.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_tagCombine $labelOneList $labelTwoList $outputList
