#include "PointerAnalysis.h"

using namespace llvm;
using namespace cs380c;

static const char* noopFuncs[] = {
	"log", "log10", "exp", "exp2", "exp10", "strcmp", "strncmp", "strlen",
	"atoi", "atof",	"atol", "atoll", "remove", "unlink", "rename", "memcmp", "free",
	"execl", "execlp", "execle", "execv", "execvp", "chmod",
	"puts", "write", "open", "create", "truncate", "chdir", "mkdir", "rmdir",
	"read", "pipe",	"wait", "time",	"stat", "fstat", "lstat", "strtod",	"strtof",
	"strtold", "fopen", "fdopen", "fflush", "feof", "fileno", "clearerr", "rewind",
	"ftell", "ferror", "fgetc",	"fgetc", "_IO_getc", "fwrite", "fread",	"fgets",
	"ungetc", "fputc", "fputs", "putc",	"ftell", "rewind", "_IO_putc", "fseek",
	"fgetpos", "fsetpos", "printf", "fprintf", "sprintf", "vprintf", "vfprintf",
	"vsprintf", "scanf", "fscanf", "sscanf", "__assert_fail", "modf", "putchar",
	"isalnum", "isalpha", "isascii", "isatty", "isblank", "iscntrl", "isdigit",
	"isgraph", "islower", "isprint", "ispunct", "isspace", "isupper", "iswalnum",
	"iswalpha", "iswctype", "iswdigit", "iswlower", "iswspace", "iswprint",
	"iswupper", "sin", "cos", "sinf", "cosf", "asin", "acos", "tan", "atan",
	"fabs", "pow", "floor", "ceil", "sqrt", "sqrtf", "hypot", 
	"random", "tolower","toupper", "towlower", "towupper", "system", "clock",
	"exit", "abort", "gettimeofday", "settimeofday", "sleep", "ctime",
	"strspn", "strcspn", "localtime", "strftime",
	"qsort", "popen", "pclose",
	"rand", "rand_r", "srand", "seed48", "drand48", "lrand48", "srand48",
	"__isoc99_sscanf", "__isoc99_fscanf", "fclose", "close", "perror", 
	"strerror", // this function returns an extenal static pointer
	"__errno_location", "__ctype_b_loc", "abs", "difftime", "setbuf",
	"_ZdlPv", "_ZdaPv",	// delete and delete[]
	"fesetround", "fegetround", "fetestexcept", "feraiseexcept", "feclearexcept",
	"llvm.bswap.i16", "llvm.bswap.i32", "llvm.ctlz.i64",
	"llvm.lifetime.start", "llvm.lifetime.end", "llvm.stackrestore",
	"memset", "llvm.memset.i32", "llvm.memset.p0i8.i32", "llvm.memset.i64",
	"llvm.memset.p0i8.i64", "llvm.va_end",
	// The following functions might not be NOOP. They need to be removed from this list in the future
	"setrlimit", "getrlimit",
	nullptr
};

static const char* mallocFuncs[] = {
	"malloc", "valloc", "calloc",
	"_Znwj", "_ZnwjRKSt9nothrow_t", "_Znwm", "_ZnwmRKSt9nothrow_t", 
	"_Znaj", "_ZnajRKSt9nothrow_t", "_Znam", "_ZnamRKSt9nothrow_t", 
	"strdup", "strndup",
	"getenv",
	"memalign", "posix_memalign",
	nullptr
};

static const char* reallocFuncs[] = {
	"realloc", "strtok", "strtok_r", "getcwd",
	nullptr
};

static const char* retArg0Funcs[] = {
	"fgets", "gets", "stpcpy",  "strcat", "strchr", "strcpy",
	"strerror_r", "strncat", "strncpy", "strpbrk", "strptime", "strrchr", "strstr",
	nullptr
};

static const char* retArg1Funcs[] = {
	// Actually the return value of signal() will NOT alias its second argument, but if you call it twice the return values may alias. We're making conservative assumption here
	"signal",
	nullptr
};

static const char* retArg2Funcs[] = {
	"freopen",
	nullptr
};

static const char* memcpyFuncs[] = {
	"llvm.memcpy.i32", "llvm.memcpy.p0i8.p0i8.i32", "llvm.memcpy.i64",
	"llvm.memcpy.p0i8.p0i8.i64", "llvm.memmove.i32", "llvm.memmove.p0i8.p0i8.i32",
	"llvm.memmove.i64", "llvm.memmove.p0i8.p0i8.i64",
	"memccpy", "memmove", "bcopy",
	nullptr
};

static const char* convertFuncs[] = {
	"strtod", "strtof", "strtol", "strtold", "strtoll", "strtoul",
	nullptr
};

static bool lookupName(const char* table[], const char* str)
{
	for (unsigned i = 0; table[i] != nullptr; ++i)
	{
		if (strcmp(table[i], str) == 0)
			return true;
	}
	return false;
}

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

	outs() << "---------------------------------------------------------------\n";
	outs() << "---------------------------------------------------------------\n";
	outs() << "---------------------------------------------------------------\n";

	for (Module::iterator I = M.begin(), E = M.end(); 
 			I != E; ++I) {
		functs.insert( &(*I) );
	}

	objectAnalysis();

	outs() << "---------------------------------------------------------------\n";
	outs() << "---------------------------------------------------------------\n";
	outs() << "---------------------------------------------------------------\n";

	for (Module::iterator I = M.begin(), E = M.end(); 
 			I != E; ++I) {
		Function *fp = &(*I);
		outs() << "<<<<< " << fp->getName() <<" >>>>>\n";
		printX(contextFree[fp]);
		printX(local[fp]);
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
				// outs() << "alloca: " << inst->getName() << ", b=" << bound << "\n";
				changed = changed | insertB | insertT;
			} else if (isAlloca) {
				bool insertB = testAndInsert(inst, inbounds);
				// outs() << "dyn alloca: " << inst->getName() << "\n";
				changed = changed | insertB;
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
				// outs() << "getelementptr: " << inst->getName() << ", offset=" << intOff << "\n";
				bool testR = testC && (origBound > 0) && (intOff + deBound <= origBound);
				if ( testB && testT && testR ) {
					bool insertB = testAndInsert(inst, inbounds);
					bool insertT = testAndInsert(inst, transitives);
					bounds[inst] = bounds[basePtr] - (intOff);
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

bool PointerAnalysis::test(Value *v, ValueSet &set)
{
	if (set.count(v) == 0) {
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
		}

		if (callee->getName() == "valloc") {
			Value *vallocSize = call_inst->getArgOperand(0);
			if (ConstantInt *vs = dyn_cast<ConstantInt>(vallocSize)) {
				return vs->getZExtValue();
			} else
				return 0;
		}

		if (callee->getName() == "calloc") {
			Value *callocNmemb = call_inst->getArgOperand(0);
			Value *callocSize = call_inst->getArgOperand(1);
			ConstantInt *cn = dyn_cast<ConstantInt>(callocNmemb);
			ConstantInt *cs = dyn_cast<ConstantInt>(callocSize);
			if ( cn != nullptr && cs != nullptr ) {
				return cn->getZExtValue() * cs->getZExtValue();
			} else
				return 0;
		}

		if (callee->getName() == "memalign") {
			Value *mallocSize = call_inst->getArgOperand(1);
			if (ConstantInt *ms = dyn_cast<ConstantInt>(mallocSize)) {
				return ms->getZExtValue();
			} else
				return 0;
		}

		if (lookupName(reallocFuncs, callee->getName().data()) || lookupName(mallocFuncs, callee->getName().data()))
			return 0;

		
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

		if (callee->getName() == "valloc") {
			return true;
		}

		if (callee->getName() == "calloc") {
			return true;
		}

		if (callee->getName() == "memalign") {
			return true;
		}

		if (lookupName(reallocFuncs, callee->getName().data()) || lookupName(mallocFuncs, callee->getName().data()))
			return true;

		return false;
	}

	return false;
}

bool PointerAnalysis::isExternalLibrary(Instruction *inst)
{
	if (isa<CallInst>(inst)) {
		CallInst *call_inst = dyn_cast<CallInst> (inst);
		Function *callee = call_inst->getCalledFunction();

		if (lookupName(noopFuncs, callee->getName().data())
			 || lookupName(mallocFuncs, callee->getName().data())
			 || lookupName(reallocFuncs, callee->getName().data())
			 || lookupName(retArg0Funcs, callee->getName().data())
			 || lookupName(retArg1Funcs, callee->getName().data())
			 || lookupName(retArg2Funcs, callee->getName().data())
			 || lookupName(memcpyFuncs, callee->getName().data())
			 || lookupName(convertFuncs, callee->getName().data()))
			return true;
	}

	return false;
}

PointerAnalysis::ValueSet PointerAnalysis::merge(PointerAnalysis::ValueSet a, PointerAnalysis::ValueSet b)
{
	ValueSet r;
	for (auto i : a)
		r.insert(i);
	for (auto i : b)
		r.insert(i);
	return r;
}

PointerAnalysis::ValueSet PointerAnalysis::join(PointerAnalysis::ValueSet a, PointerAnalysis::ValueSet b)
{
	ValueSet r;
	for (auto i : a)
		if (b.count(i))
			r.insert(i);
	return r;
}

void PointerAnalysis::joinWith(PointerAnalysis::ValueSet a, llvm::Function *funct)
{
	auto got = local.find(funct);
	if ( got == local.end() ) {
		local.insert( std::make_pair(funct, a) );
	} else
		local[funct] = join(a, local[funct]);
}

PointerAnalysis::ValueSet PointerAnalysis::getOrInsert(const PointerAnalysis::Node &n, PointerAnalysis::ValueMap &s) {
	auto got = s.find(n);
	if ( got == s.end() ) {
		s.insert( std::make_pair(n, EmptySet) );
		got = s.find(n);
	}
	
	return got->second;
}

PointerAnalysis::ArgumentAttributes 
PointerAnalysis::objectPass(Function *funct, PointerAnalysis::ArgumentAttributes argAttrs)
{
	ArgumentAttributes ret(1);
	ret[0] = UNBOUND_ATTR;

	WorkList<Node> worklist;
	NodeSet visits;
	ValueMap exactOut, boundOut;

	ValueSet exactArgs, boundArgs;
	BoundMap exactBounds;
	
	llvm::DataLayout DL(module);

	// how to test no funct body?
	if (funct->begin() == funct->end())
		return ret;

	for (auto &globalVal: module->globals()) {
		GlobalVariable *gv = dyn_cast<GlobalVariable>(&globalVal);
		exactBounds[gv] = globalBounds[gv];
	}

	auto args = funct->arg_begin();
	for (size_t i = 0; i < funct->arg_size(); i++) {
		if (argAttrs[i] == UNBOUND_ATTR) {
			++args;
			continue;
		}

		Value *arg = &(*args);
		boundArgs.insert( arg );
		if (argAttrs[i] != DYNBOUND_ATTR) {
			exactArgs.insert( arg );
			exactBounds[arg] = argAttrs[i];
		}
		++args;
	}
	
	outs() << "********** visiting " << funct->getName() << " **********\n";
	printX(exactArgs);
	printX(boundArgs);

	Node entry = &funct->getEntryBlock();
	worklist.enqueue(entry);

	std::unordered_set<Node> exits;
	for (auto &bb: *funct)
		if ( llvm::succ_begin(&bb) == llvm::succ_end(&bb) )
			exits.insert(&bb);

	while (!worklist.empty()) {
		Node next = worklist.dequeue();
		bool changed = false;
	
		bool visited = (visits.count(next) != 0);
		visits.insert(next);

		outs() << "visiting " << next->getName() << "\n";

		ValueSet oldExact = getOrInsert(next, exactOut), oldBound = getOrInsert(next, boundOut);
		ValueSet exactTemp, boundTemp;

		if (next == entry) {
			exactTemp = merge( exactTemp, exactArgs );
			exactTemp = merge( exactTemp, globals );
			boundTemp = merge( boundTemp, boundArgs );
			boundTemp = merge( boundTemp, globals );
		} else {
			for (auto i = pred_begin(next); i != pred_end(next); ++i) {
				exactTemp = merge( exactTemp, getOrInsert(*i, exactOut) );
				boundTemp = merge( boundTemp, getOrInsert(*i, boundOut) );
			}
		}

		for (auto &i : *next) {
			Instruction *inst = &i;

			if (isa<CallInst>(inst)) {
				// do inter-procedual analysis
				// what if I am calling some external library: handle that either
				// if (is a well-known exteral function) {
					// some external library
					// analyze it at best effort
				// } else {
					// not an external function
					// compare the args to the last time we enter this callsite
					// if they are the same, skip (TODO: pull out what we've got last time)
					// otherwise analyze it again
				// }

				CallInst *call_inst = dyn_cast<CallInst> (inst);
				Function *callee = call_inst->getCalledFunction();
				if (callee == nullptr)
					continue;

				if (isExternalLibrary(inst)) {
					bool isAlloca = isAllocation(inst);
					int allocaSize = getConstantAllocSize(inst);
					if (isAlloca) {
						boundTemp.insert(inst);
						if (allocaSize) {
							exactTemp.insert(inst);
							exactBounds[inst] = allocaSize;
						}
					}

					if (isaPointer(inst)) {
						if ( lookupName(retArg0Funcs, callee->getName().data()) ) {
							Value *arg0 = call_inst->getArgOperand(0);
							if (test(arg0, boundTemp)) {
								boundTemp.insert(inst);
							}
							if (test(arg0, exactTemp)) {
								exactTemp.insert(inst);
								exactBounds[inst] = exactBounds[arg0];
							}
						}

						if ( lookupName(retArg1Funcs, callee->getName().data()) ) {
							Value *arg1 = call_inst->getArgOperand(1);
							if (test(arg1, boundTemp)) {
								boundTemp.insert(inst);
							}
							if (test(arg1, exactTemp)) {
								exactTemp.insert(inst);
								exactBounds[inst] = exactBounds[arg1];
							}
						}

						if ( lookupName(retArg2Funcs, callee->getName().data()) ) {
							Value *arg2 = call_inst->getArgOperand(2);
							if (test(arg2, boundTemp)) {
								boundTemp.insert(inst);
							}
							if (test(arg2, exactTemp)) {
								exactTemp.insert(inst);
								exactBounds[inst] = exactBounds[arg2];
							}
						}
					}

				} else if ( !callee->isVarArg()) {
					bool first = (callsites.count(call_inst) == 0);
					
					callsites.insert(call_inst);
					auto oldArgs = argsCall[call_inst];
					
					ArgumentAttributes args(call_inst->getNumArgOperands());
					for (unsigned i = 0; i < call_inst->getNumArgOperands(); i++) {
						Value *argi = call_inst->getArgOperand(i);
						if (exactTemp.count(argi))
							args[i] = exactBounds[argi];
						else if (boundTemp.count(argi))
							args[i] = DYNBOUND_ATTR;
						else
							args[i] = UNBOUND_ATTR;
							
					}

					outs() << ">>> begin call\n";
					bool changed = first || ( unEqual(args, oldArgs) );
					ArgumentAttributes r;
					if (changed) {
						r = objectPass(callee, args);
						retCall[call_inst] = r;
					} else {
						r = retCall[call_inst];
					}

					assert("r must have size 1" && r.size() == 1);
					outs() << ">>> return from call\n";

					if (r[0] == DYNBOUND_ATTR)
						boundTemp.insert( inst );
					else if (r[0] != UNBOUND_ATTR) {
						boundTemp.insert( inst );
						exactTemp.insert( inst );
						exactBounds[inst] = r[0];
					}
				}				

				continue;
				
			}
			
			if (!isaPointer(inst))
				continue;

			Type* deRefTy = inst->getType()->getContainedType(0);
			uint64_t deBound = DL.getTypeStoreSize(deRefTy);

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
					if (!test(iv, boundTemp)) {
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
				// should I convert to byte count? A: NO.
				uint64_t intOff = offset.getLimitedValue();
				// outs() << "getelementptr: " << inst->getName() << ", offset=" << intOff << "\n";
				bool testR = testC && (origBound > 0) && (intOff + deBound <= origBound);
				if ( testB && testE && testR ) {
					boundTemp.insert(inst);
					exactTemp.insert(inst);
					exactBounds[inst] = exactBounds[basePtr] - (intOff);
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
			
		changed = !visited || unEqual(exactTemp, oldExact) || unEqual(boundTemp, oldBound);

		if (changed) {
			for (auto i = succ_begin(next); i != succ_end(next); ++i) {
				worklist.enqueue(*i);
			}
		}

		exactOut[next] = exactTemp;
		boundOut[next] = boundTemp;
	}

	ArgumentAttributes retSet(exits.size());
	bool hasUnbound = false, hasDynbound = false;
	outs() << "<<< " << funct->getName() << " Returns :\n";
	int i = 0;
	for (auto bb: exits) {
		outs() << bb->getName() << ":\t";
		joinWith(boundOut[bb], funct);
		printX(boundOut[bb]);

		Instruction *term_inst = bb->getTerminator();
		ReturnInst *ret_inst = dyn_cast<ReturnInst>(term_inst);
		if (ret_inst != nullptr) {
			Value *retVal = ret_inst->getReturnValue();
			if ( retVal == nullptr || (!exactOut[bb].count(retVal) && !boundOut[bb].count(retVal)) ) {
				hasUnbound = true;
				break;
			}

			if (exactOut[bb].count(retVal))
				retSet[i++] = exactBounds[retVal];
			else {
				retSet[i++] = DYNBOUND_ATTR;
				hasDynbound = true;
			}
		}
	}
	outs() << "<<<\n";

	ArgumentAttribute ret0 = retSet[0];
	for (int i = 0; i < retSet.size(); i++)
		if (retSet[i] != ret0)
			hasDynbound = true;

	if (hasUnbound)
		ret[0] = UNBOUND_ATTR;
	else if (hasDynbound)
		ret[0] = DYNBOUND_ATTR;
	else {
		ret[0] = ret0;
	}

	if (!funct->getReturnType()->isPointerTy()) {
		assert( "If is not pointer muxt be unbound" &&
			(ret[0] == UNBOUND_ATTR) );
	}

	printX(ret);

	return ret;
}


bool PointerAnalysis::unEqual (const ValueSet &a, const ValueSet &b) {
	if (a.size() != b.size())
		return true;

	for (auto const& e : a)
		if (b.count(e) == 0)
			return true;

	return false;
}

bool PointerAnalysis::unEqual (const ArgumentAttributes &a, const ArgumentAttributes &b) {
	if (a.size() != b.size())
		return true;

	for (int i = 0; i < a.size(); i++)
		if (a[i] != b[i])
			return true;

	return false;
}

void PointerAnalysis::objectAnalysis()
{
	llvm::DataLayout DL(module);

	for (auto &globalVal: module->globals()) {
		GlobalVariable *gv = dyn_cast<GlobalVariable>(&globalVal);
		Type *gvt = gv->getType();
		assert(gvt->isPointerTy());
		globals.insert(gv);
		globalBounds[gv] = DL.getTypeStoreSize(gvt->getContainedType(0));
	}

	Function *mainfp = module->getFunction("main");
	if (mainfp == nullptr)
		return;

	ArgumentAttributes args( mainfp->arg_size() );
	for (size_t i = 0; i < mainfp->arg_size(); i++) {
		args[i] = UNBOUND_ATTR;
	}

	objectPass(mainfp, args);
}

void PointerAnalysis::printX(InstSet &set) {
	outs() << "\tcnt=" << set.size() << " { ";
	for (auto v: set)
		outs() << v->getName() << " ";
	outs() << "}\n";
}

void PointerAnalysis::printX(ValueSet &set) {
	int cnt = 0;
	outs() << "\tcnt=" << set.size() << " { ";
	for (auto v: set) {
		outs() << v->getName() << " ";
		if (!globals.count(v))
			cnt++;
	}
	outs() << "} local=" << cnt << "\n";
}

void PointerAnalysis::printX(PointerAnalysis::ArgumentAttributes &set) {
	outs() << "\tcnt=" << set.size() << " { ";
	outs() << set[0] << " ";
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
