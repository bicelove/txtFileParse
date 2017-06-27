mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='../data/multiTag170103/property15class_SingleTag_161228.txt'
outputList='../data/multiTag170103/property15class_SingleTag_170103.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_deal15ClassPropertyDataTo13Class $szQueryList $outputList
