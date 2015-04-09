#ifndef CS380C_ASSIGNMENT1_H
#define CS380C_ASSIGNMENT1_H


#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/Constants.h>
#include <llvm/Analysis/CallGraph.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>

#include "llvm/Pass.h"
#include "llvm/IR/Instructions.h"

namespace cs380c
{

class MemoryAnnotator: public llvm::ModulePass {
private:
	// Private field declaration here
	llvm::Module *module;
	llvm::Type *VoidTy, *VoidPtrTy, *SizeTy, *Int32Ty;
	llvm::Function *allocaTrack, *deallocaTrack, *mallocTrack, *freeTrack, *storeTrack, *loadTrack;

	void annotateFunction(llvm::Function&);
	void annotateBasicBlock(llvm::BasicBlock&);
	void annotateInstruction(llvm::Instruction&);

	void setEnv(llvm::Module& M);
	unsigned getStrideWidth(llvm::Type *type);
	void annotateAllocaInst(llvm::AllocaInst *inst);

public:
	static char ID;
	MemoryAnnotator() : llvm::ModulePass(ID) {}

	bool runOnModule(llvm::Module&);

	// We don't modify the program, so we preserve all analyses
	void getAnalysisUsage(llvm::AnalysisUsage &AU) const;
};

}

#endif
