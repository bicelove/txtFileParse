mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
labelOneList='../data/classAndPropertyCombine/outputList_tagsCombine_170113.txt'
labelTwoList='../data/classAndPropertyCombine/170113new.txt'
outputList='../data/classAndPropertyCombine/outputList_single_170113.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_combineSingleAndMultiData $labelOneList $labelTwoList $outputList
