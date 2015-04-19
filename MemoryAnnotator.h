#ifndef CS380C_ASSIGNMENT1_H
#define CS380C_ASSIGNMENT1_H


#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/ValueHandle.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/ValueMapper.h"
#include "llvm/Transforms/Utils/Cloning.h"

#include "llvm/Pass.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/DataLayout.h"

#include <unordered_set>
#include <unordered_map>

namespace cs380c
{

class MemoryAnnotator: public llvm::ModulePass {
private:
	using InstSet = std::unordered_set<llvm::Instruction*>;

	// Private field declaration here
	llvm::Module *module;
	llvm::Type *VoidTy, *VoidPtrTy, *SizeTy, *Int32Ty;
	llvm::Function *allocaStackTrack, *saveStackTrack, *restoreStackTrack, *mallocTrack, *freeTrack, *storeTrack, *loadTrack;

	void annotateFunction(llvm::Function&);
	void annotateBasicBlock(llvm::BasicBlock&);
	void annotateInstruction(llvm::Instruction&);

	void setEnv(llvm::Module& M);
	unsigned getStrideWidth(llvm::Type *type);
	uint64_t getArraySize(llvm::Type *type);
	llvm::CastInst *cast2SizeTy(llvm::Value *value);

	void annotateAllocaInst(llvm::AllocaInst *inst);
	void annotateMalloc(llvm::CallInst *inst);
	void annotateFree(llvm::CallInst *inst);
	void annotateLoad(llvm::LoadInst *inst);
	void annotateStore(llvm::StoreInst *inst);
	void annotateStackSave(llvm::CallInst *inst);
	void annotateStackRestore(llvm::CallInst *inst);

	void cloneFunction(llvm::Function *funct);

public:
	static char ID;
	MemoryAnnotator() : llvm::ModulePass(ID) {}

	bool runOnModule(llvm::Module&);

	// We don't modify the program, so we preserve all analyses
	void getAnalysisUsage(llvm::AnalysisUsage &AU) const;
};

}

#endif
