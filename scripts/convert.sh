# sh convert.sh ${pair-of-mm} ${origin-file-path} ${output-file-path}
# example: sh convert.sh 1000
# example: sh convert.sh 1000 ../testData/inverted.txt ../testData/test.txt
rm -f ../testData/test.txt

g++ convert_union_table2.cpp -o convert

if [ -z "$2" ]; then
  ./convert $1 ../testData/inverted.txt ../testData/test.txt
else
  ./convert $1 $2 $3
fi

rm -rf converst