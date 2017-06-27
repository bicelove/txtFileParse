mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='../data/multiTag170103/outputList_MultiTagData_170103_temp.txt'
svPath='/home/in66/data/cloth/170103Property/'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_mkDataDir $szQueryList $svPath
