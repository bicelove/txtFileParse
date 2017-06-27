mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='/home/in66/data/cloth/170214txt/waitao_clear.txt'
srcDataList='/home/in66/data/cloth/170214txt/val_clear19.txt'
outputDataList='/home/in66/data/cloth/170214txt/val_clear19_27.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_getChenshanData $szQueryList $srcDataList $outputDataList
