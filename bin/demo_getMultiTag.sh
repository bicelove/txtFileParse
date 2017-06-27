mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='../data/jingdong/jd_cutword_all.txt'
similarIDList='../data/jingdong/161221/list_id_simdetect_161221.txt'
srcIDList='../data/jdImg_170103_class.txt'
outputList='../data/outputList_170103.txt'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_getMultiTag $szQueryList $similarIDList $srcIDList $outputList
