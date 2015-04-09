CXXFLAGS = $(shell llvm-config --cxxflags) -fno-rtti

all: mannotate.so

MemoryAnnotator.o: MemoryAnnotator.cpp MemoryAnnotator.h Makefile
	$(CXX) $(CXXFLAGS) MemoryAnnotator.cpp -c -o $@
mannotate.so: MemoryAnnotator.o
	$(CXX) -shared $^ -o $@
clean:
	rm -f *.o *.so
