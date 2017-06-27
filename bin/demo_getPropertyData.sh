mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='../data/jingdong/jd_cutword_all.txt'
imgList='../data/jingdong/jdImgAll.txt'
outputList='../data/propertyIMg_161229.txt'
svPath='/home/tutu/data/imageCloth/jdImgAll/'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 

./Demo_getPropertyData $szQueryList $imgList $outputList $svPath
