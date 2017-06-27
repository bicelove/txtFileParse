mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='/home/in66/programs/csvParse/multiTagParse/data/contractTagsName/oldLabels.csv'
newTagsList='/home/in66/programs/csvParse/multiTagParse/data/contractTagsName/srcLabels.csv'
outputDeleteTagsList='/home/in66/programs/csvParse/multiTagParse/data/contractTagsName/deleteLabels.csv'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_contractDeleteTags $szQueryList $newTagsList $outputDeleteTagsList
