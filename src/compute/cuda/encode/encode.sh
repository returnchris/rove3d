rm ../kernels/*

nvcc -fatbin -o ../kernels/draft.fatbin \
-Xptxas="-v" \
../draft.cu \
-arch=compute_30 -code=compute_30,sm_30,sm_32,sm_35,sm_50,sm_52,sm_53

cd ../kernels

xxd -i draft.fatbin >> module.fatbin.h

sed -i '.backup' 's/draft/module/g' module.fatbin.h

rm draft.fatbin
rm *backup

cd ../encode

clang++ -std=c++14 -O3 \
-o checksum \
checksum.cpp

./checksum
