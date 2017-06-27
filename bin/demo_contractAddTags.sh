mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
szQueryList='/home/in66/programs/csvParse/multiTagParse/data/contractTagsName/srcLabels.csv'
newTagsList='/home/in66/programs/csvParse/multiTagParse/data/contractTagsName/newLabels.csv'
outputOldTagsList='/home/in66/programs/csvParse/multiTagParse/data/contractTagsName/oldLabels.csv'
outputAddTagsList='/home/in66/programs/csvParse/multiTagParse/data/contractTagsName/addLabels.csv'

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_contractTags $szQueryList $newTagsList $outputOldTagsList $outputAddTagsList
