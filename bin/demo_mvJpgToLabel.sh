mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='../data/deepfashion/list_category_img.txt'
srcImgPath='/home/in66/Downloads/DeepFashion/Category_and_Attribute_Prediction_Benchmark/cropImg/'
saveImgPath='/home/in66/Downloads/DeepFashion/Category_and_Attribute_Prediction_Benchmark/sameLabelImg/'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_mvJpgToLabel $szQueryList $srcImgPath $saveImgPath
