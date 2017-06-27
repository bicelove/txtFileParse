mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
dataList='../data/0914_3data/scoredata.txt '
keyfile='../keyfile/'
svPath='/home/in66/data/cloth/170227class/0914_3data_rest/'

rm -r $svPath
mkdir $svPath

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_mvScore $dataList $keyfile $svPath
