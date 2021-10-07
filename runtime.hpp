#ifndef __RUNTIME_HPP__
#define __RUNTIME_HPP__

#include "llvm/IR/Type.h"
#include <llvm/IR/Value.h>
#include "llvm/IR/Instructions.h"

struct ValueEntry {
    llvm::Type       * varType;
    llvm::Value      * varValue;
    llvm::AllocaInst * varAlloca;
    ValueEntry () {};
    ValueEntry(llvm::Type *t, llvm::Value *v, llvm::AllocaInst *a) : varType(t), varValue(v), varAlloca(a) {}; 
};

class RuntimeScope {
public:
    RuntimeScope() {}

    void addVariable(std::string name, llvm::Type *Ty, llvm::Value *Val, llvm::AllocaInst * Alloca){
        if (LocalVariables.find(name) != LocalVariables.end()) yyerror("Variable already declared");
        LocalVariables[name] = ValueEntry(Ty, Val, Alloca);
    }

    ValueEntry *lookupVar(std::string c) {
        if (LocalVariables.find(c) == LocalVariables.end()) {return nullptr;}
        return &LocalVariables[c];
    }
private:
    std::map<std::string, ValueEntry> LocalVariables;
};

class RuntimeTable {
public:
    RuntimeTable() {}
    ~RuntimeTable () {}
    ValueEntry *lookup(std::string c) {
        for (auto i = scopes.rbegin(); i!= scopes.rend(); i++){
            ValueEntry *e = i->lookupVar(c);
            if (e!= nullptr) return e;
        }
        return nullptr;
    }

    void insert(std::string c, llvm::Type* t, llvm::Value *Val, llvm::AllocaInst * Alloca){
        scopes.back().addVariable(c, t, Val, Alloca);
    }

    void openScope(){
        scopes.push_back(RuntimeScope());
    }

    void closeScope(){
        scopes.pop_back();
    }

private:
    //llvm::Function *function;
    std::vector<RuntimeScope> scopes;
};

extern RuntimeTable rt;




#endif