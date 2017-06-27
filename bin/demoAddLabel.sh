mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
multiTagList='../data/jingdong/161221/multiTagImgs1_161221.txt'
allDataList='/home/in66/data/cloth/161221/jdImg_161221.txt'
mapIDList='../data/jingdong/161221/map_id_similar_src_161221.txt'
newMutiTagList='../data/jingdong/newMultiTagImgs1_161221.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_mainboby $multiTagList $allDataList $mapIDList $newMutiTagList
