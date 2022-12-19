# sh convert.sh ${pair-of-mm} ${origin-file-path} ${output-file-path}
# example: sh convert.sh 10 ../testData/inverted.txt ../testData/test.txt
g++ convert_union_table.cpp -o run -O3
./run $1 $2 $3
rm -rf run