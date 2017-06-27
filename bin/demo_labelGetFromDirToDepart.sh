mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='../data/Dict_Image_Tags_Output.txt'
outputList='../data/Dict_Image_Tags_Output_depart.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_labelGetFromDirToDepart $szQueryList $outputList
