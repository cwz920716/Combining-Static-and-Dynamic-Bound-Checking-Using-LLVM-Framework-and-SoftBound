#include "PointerAnnlysis.h"

using namespace llvm;
using namespace cs380c;

void PointerAnalysis::setEnv()
{
	// llvm::DataLayout *DL = DL(module);
}

void PointerAnalysis::isaPointer(Instruction *inst)
{
	return inst->getType()->
}

void PointerAnnlysis::localAnalysis(Function *funct) 
{
	// transitive is the (inbound) pointers with constant, static bound
	InstSet inbounds, transitives;
	BoundMap bounds;
	bool changed = false;

	for(inst_iterator i = inst_begin(funct), e = inst_end(funct); i != e; ++i) {
		Instruction* inst = &*i;
		if (isaPointer(inst)) {
			// handle a Pointer
			if (isa<AllocaInst>(inst)) {
				changed = changed ||
						(getOrInsert(inst, inbounds)) || 
						(getOrInsert(inst, transitives));
			}

			if ( isa<PHINode>(inst) ) {
				// possibly an inbound, but never transitive
			}

			if ( isa<CallInst> ) {
				// malloc with constant == alloca
				if ()
			}

			if ( isa<GetElementPtrInst>() ) {
				// inbound && transitive && offset in range => inbound && transitive
			}

			if ( isa<CastInst>() ) {
				// inbound && transitive && bound decreased => inbound && transitive
			
			}

			// default case: nothing change
		}
	}
}

bool PointerAnalysis::getOrInsert(Instrunction *inst, InstSet &set)
{
	if (set.count(inst) == 0) {
		set.insert(inst);
		return true;
	}

	return false;
}

uint64_t PointerAnalysis::getAllocaArraySize(AllocaInst *alloca_inst)
{
	Value* intBound = alloca_inst->getArraySize();
	if (ConstantInt *ib = dyn_cast<ConstantInt>(intBound))
		return ib->->getZExtValue();
	else
		return 0;
}

uint64_t PointerAnalysis::getConstantAllocSize(Instrunction *inst)
{
	if (isa<AllocaInst>) {
		AllocaInst *alloca_inst = dyn_cast<AllocaInst>(inst);
		Type *allocatedTy = alloca_inst->getAllocatedType();
		uint64_t stride = DL->getTypeStoreSize(allocatedTy);
		uint64_t length = getAllocaArraySize(alloca_inst);
		return stride * length;
	}

	if (isa<CallInst>) {
		CallInst *call_inst = dyn_cast<CallInst> (inst);
		Function *callee = call_inst->getCalledFunction();

		if (callee == nullptr) {
			return 0;
		}

		if (callee->getName() == "malloc") {
			Value *mallocSize = inst->getArgOperand(0);
			if (ConstantInt *ms = dyn_cast<ConstantInt>(mallocSize)) {
				return ms->getZExtValue();
			} else
				return 0;
		} else {
			return 0;
		}
	}

	return 0;
}
