mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='/home/in66/programs/csvParse/multiTagParse/data/hatsData/srcHats.txt'
srcDataList='/home/in66/programs/csvParse/multiTagParse/data/hatsData/otherhat.txt'
outputDataList='/home/in66/programs/csvParse/multiTagParse/data/hatsData/other_hats.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_combineMaoziData $szQueryList $srcDataList $outputDataList
