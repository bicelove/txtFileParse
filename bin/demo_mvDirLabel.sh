mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
dataList='/home/in66/cloth54val.txt'
svPath='/home/in66/cloth54val_mvDir.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_mvScoreName $dataList $svPath
