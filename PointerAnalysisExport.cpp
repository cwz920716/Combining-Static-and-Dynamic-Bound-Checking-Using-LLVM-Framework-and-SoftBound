
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/ValueHandle.h"
#include "llvm/IR/InstIterator.h"
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

using namespace llvm;
using namespace std;

class PointerAnalysisExport {
private:
	using Node = llvm::BasicBlock *;
	using InstSet = std::unordered_set<llvm::Instruction*>;
	using BoundMap = std::unordered_map< llvm::Value *, uint64_t >;
	using LocalSummary = std::unordered_map< llvm::Function *, InstSet >;
	
	// Private field declaration here
	llvm::Module *module;
	LocalSummary local;

	void setEnv(llvm::Module& M);
	bool isAllocation(llvm::Instruction *inst);
	
public:
	PointerAnalysisExport() {}

	bool init(llvm::Module&);
	// We don't modify the program, so we preserve all analyses
	bool isInbound(Value *v);
};


void PointerAnalysisExport::setEnv(Module& M)
{
	module = &M;
}

bool PointerAnalysisExport::init(Module& M)
{
	setEnv(M);

	return true;
}

bool PointerAnalysisExport::isInbound(Value *v)
{
	Instruction *inst = dyn_cast<Instruction>(v);
	return inst != nullptr && isAllocation(inst);
}

bool PointerAnalysisExport::isAllocation(Instruction *inst)
{
	llvm::DataLayout DL(module);

	if (isa<AllocaInst>(inst)) {
		return true;
	}

	if (isa<CallInst>(inst)) {
		CallInst *call_inst = dyn_cast<CallInst> (inst);
		Function *callee = call_inst->getCalledFunction();

		if (callee == nullptr) {
			return false;
		}

		if (callee->getName() == "malloc") {
			return true;
		}

		return false;
	}

	return false;
}

