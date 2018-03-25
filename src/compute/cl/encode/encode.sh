cp -p ../draft.cl obsf_draft.cl

./obsf.sh

clang++ -std=c++14 -O3 \
-o encode \
-Wno-format-security \
encode.cpp

./encode

clang++ -std=c++14 -O3 \
-o checksum \
checksum.cpp

./checksum

rm obsf_draft.cl
