mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='../data/property/train.txt'
outputGrain='../data/property/train/trainGrain.txt'
outputColor='../data/property/train/trainColor.txt'
outputOther='../data/property/train/trainOther.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_departPropertyData $szQueryList $outputGrain $outputColor $outputOther
