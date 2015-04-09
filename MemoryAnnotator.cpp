#include "MemoryAnnotator.h"

using namespace llvm;
using namespace cs380c;

bool MemoryAnnotator::runOnModule(Module& M)
{
	setEnv(M);

	// Remove the following line and before you write down your own codes
	for (Module::iterator I = M.begin(), E = M.end(); 
 			I != E; ++I) {
		annotateFunction(*I);
	}

	return false;
}

void MemoryAnnotator::setEnv(Module& M)
{
	module = &M;

	VoidTy = Type::getVoidTy(module->getContext());
	VoidPtrTy = PointerType::getUnqual(Type::getInt8Ty(module->getContext()));
	SizeTy = Type::getInt64Ty(module->getContext());
	Int32Ty = Type::getInt32Ty(module->getContext());

	Type *types[4] = {VoidPtrTy, Int32Ty, SizeTy, SizeTy};
	FunctionType *trackStackAllocaTy = FunctionType::get( VoidTy, types, false );
	allocaTrack = Function::Create( trackStackAllocaTy, Function::ExternalLinkage, "__track_stack_allocation", module );
	// module->getOrInsertFunction("__track_stack_allocation", allocaTrack);
}

void MemoryAnnotator::annotateFunction(Function& F)
{
	// Annotate Function
	for (Function::iterator I = F.begin(), E = F.end(); 
			I != E; ++I) {
		annotateBasicBlock(*I);
	}
}

void MemoryAnnotator::annotateBasicBlock(BasicBlock& BB)
{
	// Annotate Basic Block
	for (BasicBlock::iterator I = BB.begin(), E = BB.end(); 
 			I != E; ++I) {
		annotateInstruction(*I);
	}
}

unsigned MemoryAnnotator::getStrideWidth(Type *type) 
{
	if (isa<IntegerType>(type)) {
		IntegerType *int_type = dyn_cast<IntegerType>(type);
		return int_type->getBitWidth();
	}

	outs() << "WARNING: non-integer types require handling of struct/floating types\n";

	return 0;
}

void MemoryAnnotator::annotateAllocaInst(AllocaInst *inst)
{
	outs() << "----" << *inst << "----\n";

	Type *allocatedTy = inst->getAllocatedType();
	unsigned align = inst->getAlignment();
	Value *arraySize = inst->getArraySize();
	Constant *alignment = ConstantInt::get( SizeTy, align );
	Constant *stride = ConstantInt::get( SizeTy, getStrideWidth(allocatedTy) );

	if (!inst->isStaticAlloca()) {
		outs() << "WARNING: non-static alloca require handling of stack save/restore\n";
		CastInst *castArg2 = CastInst::CreateIntegerCast (arraySize, Int32Ty, false);
		castArg2->insertBefore(inst);
		arraySize = castArg2;
	}

	CastInst *cast = CastInst::CreatePointerCast (inst, VoidPtrTy);
	cast->insertAfter(inst);
	
	Value *args[4] = {cast, arraySize, stride, alignment};
	ArrayRef<Value *> argArray(args, 4);
	outs() << *cast << " " << *arraySize << " " << *stride << " " << *alignment << "\n";
	CallInst *tracker = CallInst::Create( module->getFunction("__track_stack_allocation"), argArray );
	tracker->insertAfter(cast);
	outs() << "----------------------------\n";
}

void MemoryAnnotator::annotateInstruction(Instruction& I)
{
	Instruction *inst = &I;
	if (isa<AllocaInst>(inst)) {
		AllocaInst *alloca = dyn_cast<AllocaInst> (inst);
		annotateAllocaInst(alloca);
	}

}

void MemoryAnnotator::getAnalysisUsage(AnalysisUsage &AU) const
{
	AU.setPreservesCFG();
}

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char MemoryAnnotator::ID = 0;

// Register the pass name to allow it to be called with opt:
// See http://llvm.org/releases/3.5.1/docs/WritingAnLLVMPass.html#running-a-pass-with-opt for more info.
static RegisterPass<MemoryAnnotator> X("mannotate", "Memory + Source line number annotator");
