#include "PointerAnalysis.h"

using namespace llvm;
using namespace cs380c;

void PointerAnalysis::setEnv(Module& M)
{
	module = &M;
}

bool PointerAnalysis::runOnModule(Module& M)
{
	setEnv(M);

	// Remove the following line and before you write down your own codes
	for (Module::iterator I = M.begin(), E = M.end(); 
 			I != E; ++I) {
		contextFreeAnalysis( &(*I) );
	}

	return false;
}

bool PointerAnalysis::isaPointer(Instruction *inst)
{
	return inst->getType()->isPointerTy();
}

// Arguments: Out-of-Bound
// ConstantPointer: Not exist normally unless for NULL, NULL is OoB
// Global Values: OoB for simplicity
void PointerAnalysis::contextFreeAnalysis(Function *funct) 
{
	// transitive is the (inbound) pointers with constant, static bound
	InstSet inbounds, transitives;
	BoundMap bounds;
	bool changed = false;
	llvm::DataLayout DL(module);

	do {
	changed = false;

	for(inst_iterator i = inst_begin(funct), e = inst_end(funct); i != e; ++i) {
		Instruction* inst = &*i;
		if (isaPointer(inst)) {
			// handle a Pointer
			Type* deRefTy = inst->getType()->getContainedType(0);
			uint64_t deBound = DL.getTypeStoreSize(deRefTy);

			bool isAlloca = isAllocation(inst);
			uint64_t bound = getConstantAllocSize(inst);


			if (isAlloca && bound) {
				bool insertB = testAndInsert(inst, inbounds);
				bool insertT = testAndInsert(inst, transitives);
				bounds[inst] = bound;
				outs() << "alloca: " << inst->getName() << ", b=" << bound << "\n";
				changed = changed | insertB | insertT;
			} else if (isAlloca) {
				bool insertB = testAndInsert(inst, inbounds);
				outs() << "dyn alloca: " << inst->getName() << "\n";
				changed = changed | insertB;
			}

			// TODO: handle dynamic allocated inst
			Type* deRefTy = inst->getType()->getContainedType(0);
			uint64_t deBound = DL.getTypeStoreSize(deRefTy);

			if ( isa<PHINode>(inst) ) {
				// possibly an inbound, but never transitive
				const PHINode *phi = dyn_cast<PHINode>(inst);
				bool allInbound = true;
				for (unsigned i = 0; i < phi->getNumIncomingValues(); i++) {
					Value *iv = phi->getIncomingValue(i);
					if (!test(iv, inbounds)) {
						allInbound = false;
						break;
					}
				}
				if (allInbound) {
					bool insertB = testAndInsert(inst, inbounds);
					changed = changed | insertB;
				}
				
			}

			if ( isa<GetElementPtrInst>(inst) ) {
				// inbound && transitive && offset in range => inbound && transitive
				GetElementPtrInst *gep_inst = dyn_cast<GetElementPtrInst>(inst);
				Value *basePtr = gep_inst->getPointerOperand();
				uint64_t origBound = bounds[basePtr];
				APInt offset(64, false);
				bool testB = test(basePtr, inbounds);
				bool testT = test(basePtr, transitives);
				bool testC = gep_inst->accumulateConstantOffset(DL, offset);
				// should I convert to byte count?
				uint64_t intOff = offset.getLimitedValue();
				outs() << "getelementptr: " << inst->getName() << ", offset=" << intOff << "\n";
				bool testR = testC && (origBound > 0) && (intOff + deBound <= origBound);
				if ( testB && testT && testR ) {
					bool insertB = testAndInsert(inst, inbounds);
					bool insertT = testAndInsert(inst, transitives);
					bounds[inst] = bounds[basePtr] - (intOff + deBound);
					changed = changed | insertB | insertT;
				}
			}

			if ( isa<CastInst>(inst) ) {
				// inbound && transitive && bound decreased => inbound && transitive
				CastInst *cast_inst = dyn_cast<CastInst>(inst);
				if (cast_inst->getSrcTy()->isPointerTy()) {
					Value *srcPtr = cast_inst->getOperand(0);
					bool testB = test(srcPtr, inbounds);
					bool testT = test(srcPtr, transitives);
					uint64_t origBound = bounds[srcPtr];
					bool testR = (origBound > 0) && (deBound <= origBound);
					if ( testB && testT && testR ) {
						bool insertB = testAndInsert(inst, inbounds);
						bool insertT = testAndInsert(inst, transitives);
						bounds[inst] = origBound;
						changed = changed | insertB | insertT;
					}
				}
			
			}

			// TODO: handle load/store
			// default case: nothing change
		}
	}

	} while (changed);

	outs() << funct->getName() << ":\t";
	printX(inbounds);
	contextFree[funct] = inbounds;
}

bool PointerAnalysis::testAndInsert(Instruction *inst, InstSet &set)
{
	if (!test(inst, set)) {
		set.insert(inst);
		return true;
	}

	return false;
}

bool PointerAnalysis::test(Value *v, InstSet &set)
{
	Instruction *inst = dyn_cast<Instruction>(v);
	if (!inst) {
		return false;
	}

	if (set.count(inst) == 0) {
		return false;
	}

	return true;
}

uint64_t PointerAnalysis::getAllocaArraySize(AllocaInst *alloca_inst)
{
	Value* intBound = alloca_inst->getArraySize();
	if (ConstantInt *ib = dyn_cast<ConstantInt>(intBound))
		return ib->getZExtValue();
	else
		return 0;
}

uint64_t PointerAnalysis::getConstantAllocSize(Instruction *inst)
{
	llvm::DataLayout DL(module);

	if (isa<AllocaInst>(inst)) {
		AllocaInst *alloca_inst = dyn_cast<AllocaInst>(inst);
		Type *allocatedTy = alloca_inst->getAllocatedType();
		uint64_t stride = DL.getTypeStoreSize(allocatedTy);
		uint64_t length = getAllocaArraySize(alloca_inst);
		return stride * length;
	}

	if (isa<CallInst>(inst)) {
		CallInst *call_inst = dyn_cast<CallInst> (inst);
		Function *callee = call_inst->getCalledFunction();

		if (callee == nullptr) {
			return 0;
		}

		if (callee->getName() == "malloc") {
			Value *mallocSize = call_inst->getArgOperand(0);
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

bool PointerAnalysis::isAllocation(Instruction *inst)
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

ValueSet PointerAnalysis::merge(ValueSet a, ValueSet b)
{
	ValueSet r;
	for (auto i : a)
		r.insert(i);
	for (auto i : b)
		r.insert(i);
	return r;
}

ValueSet getOrInsert(const Node &n, ValueMap &s) {
	auto got = s.find(n);
	if ( got == s.end() ) {
		s.insert( std::make_pair(n, EmptySet) );
		got = s.find(n);
	}
	
	return got->second;
}

void PointerAnalysis::objectPass(Function *funct, ValueSet exactArgs, ValueSet boundArgs)
{
	WorkList<Node> worklist;
	NodeSet visits;
	ValueMap exactIn, exactOut, boundIn, boundOut;
	
	Node entry = &funct->getEntryBlock();
	worklist.enqueue(entry);

	while (!worklist.empty()) {
		Node next = worklist.dequeue();
		bool changed = false;
	
		bool visited = (visit.count(next) != 0);
		visit.insert(next);

		ValueSet exactTemp, boundTemp;

		if (next == entry) {
			exactTemp = merge( exactTemp, exactArgs );
			exactTemp = merge( exactTemp, globals );
			boundTemp = merge( boundTemp, boundArgs );
		} else {
			for (auto i = pred_begin(next); i != pred_end(next); ++i) {
				exactTemp = merge( exactTemp, getOrInsert(*i, exactOut) );
				boundTemp = merge( boundTemp, getOrInsert(*i, boundOut) );
			}
		}

		for (auto const &i : *next) {
			const Instruction *inst = &i;

			if (isa<CallInst>(inst)) {
				// do inter-procedual analysis
				// what if I am calling some external library: handle that either
				// if () {
					// some external library
					// analyze it at best effort
				// } else {
					// not an external function
					// compare the args to the last time we enter this callsite
					// if they are the same, skip
					// otherwise call it again
				// }

				continue;
				
			}
			
			if (!isaPointer(inst))
				continue;

			// handle a Pointer
			if ( isa<AllocaInst>(inst) ) {
				AllocaInst *alloca_inst = dyn_cast<AllocaInst>(inst);
				Type *allocatedTy = alloca_inst->getAllocatedType();
				uint64_t stride = DL.getTypeStoreSize(allocatedTy);
				uint64_t length = getAllocaArraySize(alloca_inst);
				boundTemp.insert(inst);
				if (length) {
					exactTemp.insert(inst);
					exactBounds[inst] = stride * length;
				}
				
			}

			if ( isa<PHINode>(inst) ) {
				// possibly an inbound, but never transitive
				const PHINode *phi = dyn_cast<PHINode>(inst);
				bool allInbound = true;
				for (unsigned i = 0; i < phi->getNumIncomingValues(); i++) {
					Value *iv = phi->getIncomingValue(i);
					if (!test(iv, inbounds)) {
						allInbound = false;
						break;
					}
				}
				if (allInbound) {
					boundTemp.insert(inst);
				}
				
			}

			if ( isa<GetElementPtrInst>(inst) ) {
				// inbound && exact && offset in range => inbound && transitive
				GetElementPtrInst *gep_inst = dyn_cast<GetElementPtrInst>(inst);
				Value *basePtr = gep_inst->getPointerOperand();
				uint64_t origBound = exactBounds[basePtr];
				APInt offset(64, false);
				bool testB = test(basePtr, boundTemp);
				bool testE = test(basePtr, exactTemp);
				bool testC = gep_inst->accumulateConstantOffset(DL, offset);
				// should I convert to byte count?
				uint64_t intOff = offset.getLimitedValue();
				outs() << "getelementptr: " << inst->getName() << ", offset=" << intOff << "\n";
				bool testR = testC && (origBound > 0) && (intOff + deBound <= origBound);
				if ( testB && testE && testR ) {
					boundTemp.insert(inst);
					exactTemp.insert(inst);
					exactBounds[inst] = exactBounds[basePtr] - (intOff + deBound);
				}
			}

			if ( isa<CastInst>(inst) ) {
				// inbound && transitive && bound decreased => inbound && transitive
				CastInst *cast_inst = dyn_cast<CastInst>(inst);
				if (cast_inst->getSrcTy()->isPointerTy()) {
					Value *srcPtr = cast_inst->getOperand(0);
					bool testB = test(srcPtr, boundTemp);
					bool testE = test(srcPtr, exactTemp);
					uint64_t origBound = exactBounds[srcPtr];
					bool testR = (origBound > 0) && (deBound <= origBound);
					if ( testB && testE && testR ) {
						boundTemp.insert(inst);
						exactTemp.insert(inst);
						exactBounds[inst] = origBound;
					}
				}
			
			}

			// TODO: handle load/store
			// default case: nothing change
			
		}

		if (changed) {
			for (auto i = succ_begin(next); i != succ_end(next); ++i) {
				worklist.enqueue(*i);
			}
		};
	}
}

void PointerAnalysis::objectAnalysis()
{
	llvm::DataLayout DL(module);

	for (auto globalVal: module->globals()) {
		GlobalVariable *gv = dyn_cast<GlobalVariable>(GlobalVariable);
		Type *gvt = gv->getType();
		assert(gvt->isPointerTy());
		globals.insert(gv);
		exactBounds[gv] = DL.getTypeStoreSize(gvt->getContainedType(0));
	}

	Function *main = module->getFunction("main");
	if (main != nullptr)
		return;

	objectPass(main, EmptySet, EmptySet);
}

void PointerAnalysis::printX(InstSet &set) {
	outs() << "\tcnt=" << set.size() << " { ";
	for (auto v: set)
		outs() << v->getName() << " ";
	outs() << "}\n";
}

void PointerAnalysis::getAnalysisUsage(AnalysisUsage &AU) const
{
	AU.setPreservesCFG();
}

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char PointerAnalysis::ID = 0;

// Register the pass name to allow it to be called with opt:
// See http://llvm.org/releases/3.5.1/docs/WritingAnLLVMPass.html#running-a-pass-with-opt for more info.
static RegisterPass<PointerAnalysis> X("ptranalysis", "Pointer Analysis for softbound");
