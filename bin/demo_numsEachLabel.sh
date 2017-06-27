mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='/media/in66/文档/data/细分类/data/场景.抽象/dataStatic/test.txt'
outputList='/media/in66/文档/data/细分类/data/场景.抽象/dataStatic/test_count.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_numsEachLabel $szQueryList $outputList
