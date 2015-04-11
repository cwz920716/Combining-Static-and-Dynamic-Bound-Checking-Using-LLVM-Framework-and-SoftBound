#! /bin/sh

rm -f *.ll *.bc ./input ./output build/*
make clean
clang -emit-llvm -c example/$1.c -o build/$1.bc
opt -mem2reg -instnamer build/$1.bc -o build/input.bc
llvm-dis build/input.bc -o build/input.ll
make
echo "<<<< Memory Annotate... >>>>"
opt -load ./mannotate.so -mannotate build/input.bc -o build/output.bc  
llvm-dis build/output.bc -o build/output.ll
echo "<<<< Linking... >>>>"
clang -O3 build/output.bc ./lib/libtrack.a -o build/output
objdump -d build/output > build/output-dis.ll
