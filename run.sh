#! /bin/sh

rm -f *.ll *.bc ./input ./output
make clean
clang -emit-llvm -c example/$1.c
opt -mem2reg -instnamer $1.bc -o input.bc
llvm-dis input.bc -o input.ll
make
echo ----Memory Annotate----
opt -load ./mannotate.so -mannotate input.bc -o output.bc  
llvm-dis output.bc -o output.ll
