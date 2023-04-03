# sh convert.sh ${origin-file-path} ${pair-of-mm} ${output-file-path}
# example: sh getid.sh 100
# example: sh getid.sh 100 ../testData/test.txt

g++ get_mm_id.cpp -o getid

if [ -z "$2" ]; then
  ./getid $1 ../testData/test.txt
else
  ./getid $1 $2
fi

rm -rf getid