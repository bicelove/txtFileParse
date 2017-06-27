mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='../data/jingdong/imgIDMapTemp_161221.txt'
dataList='../data/cloth161221.txt'
svPath='../data/svData/'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_mainboby $szQueryList $dataList $svPath
