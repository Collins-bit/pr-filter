# sh convert.sh ${pair-of-mm} ${origin-file-path} ${output-file-path}
# example: sh convert.sh 1000
# example: sh convert.sh 1000 ../testData/inverted.txt ../testData/test.txt

g++ convert_union_table.cpp -o convert -O3

if [ -z "$2" ]; then
  ./convert $1 ../testData/inverted.txt ../testData/test.txt
else
  ./convert $1 $2 $3
fi

rm -rf convert