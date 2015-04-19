#include "MemoryAnnotator.h"

using namespace llvm;
using namespace cs380c;

void MemoryAnnotator::buildStackEscapeSet(Function *funct) 
{
	InstSet cands, argEscape, gloabalEscape;

/*base case: allocaInst*/
	for (Function::iterator I = F.begin(), E = F.end(); 
			I != E; ++I) {
		for (BasicBlock::iterator I = BB.begin(), E = BB.end(); 
 				I != E; ++I) {
			Instruction *inst = &(*I);
			if (isa<AllocaInst>(inst)) {
				cands.insert(inst);
			}
		}
	}

/*induction case: getElementPtr, etc.*/
	bool changed;
	do {
		changed = false;
		InstSet temp = cands;
		for (auto const inst : temp) {
			for (Instruction::const_user_iterator ui = inst->user_begin(),
						ue = inst->user_end();
				ui != ue; ++ui) {
				const User *use = *ui;
				const Instruction *use_i = dyn_cast<Instruction>(use);
				if (const GetElementPtrInst *store = dyn_cast<GetElementPtrInst>(use_i)) {
					changed = true;
					cands.insert(use_i);
				}	
			}
		}
	} while (changed);


	do {
		changed = false;

	for (auto const inst : cands) {
		for (Instruction::const_user_iterator ui = inst->user_begin(),
							ue = inst->user_end();
			ui != ue; ++ui) {
			const User *use = *ui;
			const Instruction *use_i = dyn_cast<Instruction>(use);
			if (const StoreInst *store = dyn_cast<StoreInst>(use_i)) {
				Value* ptr = store->getPointerOperand();
				Value* val = store->getValueOperand();
				if (val == inst && 
					(cands.count(ptr) == 0 || globalEscape.count(ptr) != 0 )) {
					globalEscape.insert(inst);
					changed = true;
				}
			}
			if (const CallInst *call = dyn_cast<CallInst>(use_i)) {
				argEscape.insert(inst);
			}
			if (const CastInst *cast = dyn_cast<CastInst>(use_i)) {
				globalEscape.insert(inst); // be conservative, convert stack address is meaningless and suspicious
				changed = true;
			}
		}
	}

	} while (changed);
	
}
