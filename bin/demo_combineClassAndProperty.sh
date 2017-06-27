mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
labelOneList='../data/deepfashion/list_category_img.txt'
labelTwoList='../data/deepfashion/out_attr_img.txt'
outputList='../data/deepfashion/outputList_tagsCombine_deepfashion.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_combineClassAndProperty $labelOneList $labelTwoList $outputList
