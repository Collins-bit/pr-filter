# sh convert.sh ${origin-file-path} ${pair-of-mm} ${output-file-path}
# example: sh getmm.sh ../testData/union_MM.txt
# example: sh getmm.sh ../testData/union_MM.txt 1000 ../testData/test.txt

g++ get_MM_num.cpp -o getmm -O3

if [ -z "$2" ]; then
  ./getmm $1
else
  ./getmm $1 $2 $3
fi

rm -rf getmm