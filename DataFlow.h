#ifndef CS380C_ASSIGNMENT3_DATAFLOW_H
#define CS380C_ASSIGNMENT3_DATAFLOW_H

#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include <unordered_set>
#include <unordered_map>

using namespace llvm;

namespace cs380c {

template < typename T >
class WorkList {
private:
	std::unordered_set<T> s;
	
public:
	WorkList() = default;

	bool enqueue(T elem) {
		if (has(elem))
			return false;

		s.insert(elem);
		return true;
	}

	T dequeue() {
		if (empty()) {
			errs() << "deque a empty worklist!\n";
			exit(-1);
		}

		auto it = s.begin();
		T elem = *it;
		s.erase(it);
		return elem;
	}

	bool has(T elem) {
		auto got = s.find(elem);
		return got != s.end();
	}

	bool empty() {
		return s.empty();
	}

};

template < typename N, typename V >
class DataFlowAnalysis {
protected:
	using DataSet = std::unordered_map< N, V >;
	using Record = std::unordered_map< N, bool >;

	DataSet inSet, outSet;
	WorkList<N> wlist;
	bool direction;
	bool FORWARD = false, BACKWARD = true;
	V TOP, boundaryValue;

	Record recSet;

	Function *FP;
	virtual ~DataFlowAnalysis() = default;

public:
	DataFlowAnalysis() = default;

	virtual V meet(const V &a, const V &b) = 0;
	virtual bool unEqual(const V &a, const V &b) = 0;
	virtual V transfer(const N &n, const V &v) = 0;
	virtual void setup() = 0;
	virtual bool isBoundary(const N &n) = 0;

	virtual bool visitNode(const N &n) {
		bool changed = false;
		V oldIn = getOrInsert(n, inSet);
		V oldOut = getOrInsert(n, outSet);
		V newIn = TOP, newOut = TOP;

		bool seen = visited(n);

		if (direction == FORWARD) {
			newIn = meetAllPred(n);
			newOut = transfer(n, newIn);
			changed = unEqual(newOut, oldOut);
		} else {
			newOut = meetAllSucc(n);
			newIn = transfer(n, newOut);
			changed = unEqual(newIn, oldIn);
		}
		
		inSet[n] = newIn;
		outSet[n] = newOut;

		return changed || !seen;
	}

	// Override if N do not support pred_begin/end
	virtual V meetAllPred(const N &n) {
		if (isBoundary(n)) {
			return boundaryValue;
		}

		V v = TOP;
		for (auto i = pred_begin(n); i != pred_end(n); ++i) {
			v = meet( v, getOrInsert(*i, outSet) );
		}

		return v;
	}

	// Override if N do not support pred_begin/end
	virtual V meetAllSucc(const N &n) {
		if (isBoundary(n)) {
			return boundaryValue;
		}

		V v = TOP;
		for (auto i = succ_begin(n); i != succ_end(n); ++i) {
			v = meet( v, getOrInsert(*i, inSet) );
		}

		return v;
	}

	void propogate(const N &n) {
		if (direction == FORWARD) {
			for (auto i = succ_begin(n); i != succ_end(n); ++i) {
				wlist.enqueue(*i);
			}
		} else {
			for (auto i = pred_begin(n); i != pred_end(n); ++i) {
				wlist.enqueue(*i);
			}
		}
	}

	void dataFlowWalking() {
		setup();
		
		while (!wlist.empty()) {
			N next = wlist.dequeue();
			bool changed = visitNode(next);
			if (changed)
				propogate(next);
		}
	}

	V getOrInsert(const N &n, DataSet &s) {
		auto got = s.find(n);
		if ( got == s.end() ) {
			s.insert( std::make_pair(n, TOP) );
			got = s.find(n);
		}
		
		return got->second;
	}

	V get(const N &n, const DataSet &s) const {
		auto got = s.find(n);
		return got->second;
	}

	bool visited(const N &n) {
		if (recSet.find(n) == recSet.end()) {
			recSet.insert( std::make_pair(n, true) );
			return false;
		}
		else
			return true;
	}


};

bool callDefinedValue(const CallInst *cip) {
	const Function *Funct = nullptr;
	const FunctionType *FunctTy = nullptr;

	assert (cip != nullptr);
	Funct = cip->getCalledFunction();

	if (Funct == nullptr) {
		// an indirect call
		Type* ty = cip->getCalledValue()->getType();
		FunctTy = cast<FunctionType>(cast<PointerType>(ty)->getElementType());
	} else
		FunctTy = Funct->getFunctionType();

	if (FunctTy == nullptr) {
		return false; // sth is wrong, let's say it is not a def
	}

	if (FunctTy->getReturnType()->isVoidTy())
		return false;

	return true;
}

bool invokeDefinedValue(const InvokeInst *iip) {
	const Function *Funct = nullptr;
	const FunctionType *FunctTy = nullptr;

	assert (iip != nullptr);
	Funct = iip->getCalledFunction();

	if (Funct == nullptr) {
		// an indirect call
		Type* ty = iip->getCalledValue()->getType();
		FunctTy = cast<FunctionType>(cast<PointerType>(ty)->getElementType());
	} else
		FunctTy = Funct->getFunctionType();

	if (FunctTy == nullptr) {
		return false; // sth is wrong, let's say it is not a def
	}

	if (FunctTy->getReturnType()->isVoidTy())
		return false;

	return true;
}

bool hasDefinedValue(const Instruction *ip) {
	const StoreInst *sip = dyn_cast<StoreInst> (ip);
	if (sip != nullptr)
		return false;

	const AtomicCmpXchgInst *acip = dyn_cast<AtomicCmpXchgInst> (ip);
	if (acip != nullptr)
		return false;

	const AtomicRMWInst *arip = dyn_cast<AtomicRMWInst> (ip);
	if (arip != nullptr)
		return false;

	const FenceInst *fip = dyn_cast<FenceInst> (ip);
	if (fip != nullptr)
		return false;

	const CallInst *cip = dyn_cast<CallInst> (ip);
	if (cip != nullptr) {
		return callDefinedValue(cip); 
	}

	const InvokeInst *iip = dyn_cast<InvokeInst> (ip);
	if (iip != nullptr)
		return invokeDefinedValue(iip);

	if (ip->isTerminator())
		return false;

	return true;
}

class DominatorAnalysis {
private:
	using DANode = const llvm::BasicBlock *;
	using DASet = std::unordered_set< DANode >;
	using DOMAP = std::unordered_map< DANode, DASet >;

	DASet merge (DASet a, DASet b) {
		DASet res;

		for (auto const& e : a) {
			res.insert(e);
		}

		for (auto const& e : b) {
			res.insert(e);
		}

		return res;
	}

	DASet join (DASet a, DASet b) {
		DASet res;

		for (auto const& e : a) {
			if (b.count(e) == 1)
				res.insert(e);
		}

		return res;
	}

public:
	DOMAP doms;

	DominatorAnalysis() {
	}

	void setup(Function &f) {
		DANode entry = &f.getEntryBlock();
		DASet U, ENTRY;

		ENTRY.insert(entry);
		for (auto const& bb: f) {
			DANode n = &bb; 
			U.insert(n);
		}

		for (auto const& bb: f) {
			DANode n = &bb;
			if (n == entry)
				doms[n] = ENTRY;
			else 
				doms[n] = U;
		}

		bool changed = false;
		do {
			changed = false;
			for (auto const& bb: f) {
				DANode n = &bb;
				DASet set, predset = U;

				if (n == entry) 
					continue;

				set.insert(n);
				for (auto i = pred_begin(n); i != pred_end(n); ++i) {
					predset = join(predset, doms[*i]);
				}
				set = merge(set, predset);
				if (set != doms[n]) {
					changed = true;
					doms[n] = set;
				}
			}
		} while( changed == true);
	}

	bool isDominatorOf(DANode a, DANode b) {
		if (a == b)
			return false;

		if (doms.find(b) == doms.end())
			return false;
		return doms[b].count(a) != 0;
	}
};

}

#endif