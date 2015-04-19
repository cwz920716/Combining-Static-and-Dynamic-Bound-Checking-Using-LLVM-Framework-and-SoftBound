#include "MemoryAnnotator.h"

using namespace llvm;
using namespace cs380c;

class MessageUpdater: public ValueMapTypeRemapper
{
public:
    Type *remapType(Type *SrcTy)
    {
        std::map<Type *, Type *>::iterator i, e;
        for (i = foo.begin(), e = foo.end(); i != e; ++i) {
            if (SrcTy == i->first) {
                return i->second;
            }
        }

        assert(0 && "Type not found in map!");
        return 0;
    }
    std::map<Type *, Type *> foo;
};

void MemoryAnnotator::cloneFunction(Function *funct)
{
	// Annotate Function
	FunctionType *functTy = funct->getFunctionType();
	Type *retTy = funct->getReturnType();
	if (retTy == Int32Ty) {
		retTy = FatPtrTy;
	}	

	Type *args[funct->arg_size()];
	for (size_t i = 0; i < funct->arg_size() ;i++) {
		args[i] = functTy->getParamType(i);
	}
	ArrayRef<Type *> argArray(args, funct->arg_size());
	FunctionType *clonedFunctTy = FunctionType::get(retTy, argArray, false);

	Function *clone = Function::Create( clonedFunctTy, funct->getLinkage(), funct->getName() + "_clone", module );


	ValueMap<const Value *, WeakVH> ValueMap;
	auto oldArgs = funct->arg_begin();
	auto clonedArgs = clone->arg_begin();
	for (size_t i = 0; i < funct->arg_size() ;i++) {
		WeakVH nvh(&(*clonedArgs));
		ValueMap[&(*oldArgs)] = nvh;
		oldArgs++;
		clonedArgs++;
	}

	typedef SmallVector<ReturnInst *, 8> ReturnSet; 

	ReturnSet returns;
	llvm::CloneFunctionInto(clone, funct,
				ValueMap,
				false,
				returns,
				"_clone", nullptr,
				nullptr,
				nullptr);

	for (auto inst : returns) {
		Value *ret = inst->getReturnValue();
		UndefValue *undef = UndefValue::get(FatPtrTy);
		unsigned a[] = {0};
		ArrayRef<unsigned> idx(a, 1);
		InsertValueInst *first = InsertValueInst::Create(undef, ret, idx);
		first->insertBefore(inst);
		a[0] = 1;
		ArrayRef<unsigned> idx2(a, 1);
		InsertValueInst *second = InsertValueInst::Create(first, ret, idx2);
		second->insertBefore(inst);
		inst->setOperand(0, second);
	}
	
}
