#! /bin/sh

rm -Rf *.ll *.bc ./input ./output ./build
mkdir build
make clean
opt -mem2reg -instnamer ubench/$1.bc -o build/input.bc
llvm-dis build/input.bc -o build/input.ll
make
# echo "<<<< Memory Annotate... >>>>"
# opt -load ./mannotate.so -mannotate build/input.bc -o build/output.bc  
# llvm-dis build/output.bc -o build/output.ll
# echo "<<<< Linking... >>>>"
# clang -O3 build/output.bc ./lib/libtrack.a -o build/output
# objdump -d build/output > build/output-dis.ll
# gcc -O3 example/$1.c -o build/input
echo "<<<< Function Clone... >>>>"
opt -load ./fclone.so -fclone build/input.bc -o build/output.bc
llvm-dis build/output.bc -o build/output.ll
echo "<<<< Pointer Analysis... >>>>"
opt -load ./ptranalysis.so -ptranalysis build/output.bc -o build/meta-output.bc
llvm-dis build/meta-output.bc
