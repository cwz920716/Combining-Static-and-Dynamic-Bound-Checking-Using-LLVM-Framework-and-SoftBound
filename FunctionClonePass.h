#ifndef _FUNCTIONCLONEPASS_H
#define _FUNCTIONCLONEPASS_H


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
#include <stack>
#include <vector>

using namespace llvm;
using namespace std;

class FunctionClonePass: public llvm::ModulePass {
public:
	using CGNodeSet = std::unordered_set<CallGraphNode *>;
	using CallNodeIndexMap = std::unordered_map<CallGraphNode *, int>;
	using CallNodeNameMap = std::unordered_map<int, CallGraphNode *>;
	using Context = std::vector<int>;

private:
	// Private field declaration here
	llvm::Module *module;

	void setEnv(Module& M);

	// SCC algorithm
	int V;
	int count;
	int pre;
	std::vector<bool> marked;
	std::vector<int> low;
	std::vector<int> id;
	std::stack<int> stack;

	CallNodeIndexMap indexes;
	CallNodeNameMap names;

	void buildSCC(CallGraph &cg);
	void dfs(CallGraph &cg, int v);

	FunctionClonePass::Context stitch(FunctionClonePass::Context parent, int myId, int myCnt);
	std::string context2Str(FunctionClonePass::Context ctx);
	Function *getClonedFunction(Function *funct, CallGraph &cg, FunctionClonePass::Context parent);
	bool cloneMain(Function *funct, CallGraph &cg);

public:
	static char ID;
	FunctionClonePass() : llvm::ModulePass(ID) {}

	bool runOnModule(llvm::Module&);
	void getAnalysisUsage(llvm::AnalysisUsage &AU) const;
};


#endif
