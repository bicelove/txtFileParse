mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
srcDataList='/home/in66/data/cloth/txt/train_clear23_clear30.txt'
outputDataList='/home/in66/data/cloth/txt/train_clear23_30_31_32.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_mvMaoziData $srcDataList $outputDataList
