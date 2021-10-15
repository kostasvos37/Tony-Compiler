#ifndef __RUNTIME_HPP__
#define __RUNTIME_HPP__

#include "llvm/IR/Type.h"
#include <llvm/IR/Value.h>
#include "llvm/IR/Instructions.h"

class LLVMListTypes {
public:
    LLVMListTypes() {}
    ~LLVMListTypes () {}
    llvm::Type* lookup(std::string hash) {
        if (list_map.find(hash) == list_map.end()) return nullptr;
        return list_map[hash];
    }

    void insert(std::string hash, llvm::Type* t){
        if (list_map.find(hash) != list_map.end()) {
            yyerror("List type: %s already defined", hash.c_str());
        }
        list_map[hash] = t;
    }
private:
    std::map<std::string, llvm::Type*> list_map;
};

struct ValueEntry {
    llvm::Type       * varType;
    llvm::Value      * varValue;
    llvm::AllocaInst * varAlloca;
    ValueEntry () {};
    ValueEntry(llvm::Type *t, llvm::Value *v, llvm::AllocaInst *a) : varType(t), varValue(v), varAlloca(a) {}; 
};


class RuntimeBlock {
public:
    RuntimeBlock() {
        fun = nullptr;
        currentBB = nullptr;
        hasRet = false;
    }
    ~RuntimeBlock(){}

    void setFun(llvm::Function *f1){
        fun = f1;
    }
    llvm::Function *getFun(){
        return fun;
    }
    
    void addArg(std::string name, llvm::Type *type){
        args.push_back(type);
        vars[name] = type;
    }

    void addVar(std::string name, llvm::Type *type){
        vars[name] = type;
    }

    llvm::Type *getVar(std::string(name)){
        return vars[name];
    }
    
    void addAddr(std::string name, llvm::AllocaInst *alloca){
        addrs[name] = alloca;
    }

    llvm::AllocaInst *getAddr(std::string(name)){
        return addrs[name];
    }

    void addVal(std::string name, llvm::Value *v){
        values[name] = v;
    }

    llvm::Value *getVal(std::string(name)){
        return values[name];
    }

    void setBlock(llvm::BasicBlock *BB){
        currentBB = BB;
    }

    llvm::BasicBlock * getCurrentBasicBlock(){
        return currentBB;
    }

    std::vector<llvm::Type *> getArgs(){
        return args;
    }


private:
    llvm::Function *fun;
    std::vector<llvm::Type *> args;
    std::map<std::string, llvm::Type *> vars;
    std::map<std::string, llvm::Value *> values;
    std::map<std::string, llvm::AllocaInst *> addrs;
    llvm::BasicBlock *currentBB;
    bool hasRet;
};

class RuntimeScope {
public:
    RuntimeScope() {}
    ~RuntimeScope() {}

    void openRuntimeScope(){
        functions.push_back(std::map<std::string, llvm::Function *>());
    }

    void closeRuntimeScope(){
        if(functions.empty()){
            yyerror("Trying to close scope, none available!");
            return;
        }        
        functions.pop_back();
    }

    void insertFunc(std::string name, llvm::Function *Fun){
        functions.back()[name] = Fun;
    }

    llvm::Function* getFun(std::string name){
        for (auto i = functions.rbegin(); i!= functions.rend(); i++){
            if( (*i).find(name) != (*i).end() ){
                return (*i)[name];
            }
        }
        return nullptr;
    }

private:
    std::vector<std::map<std::string, llvm::Function *>> functions;
    
};

extern LLVMListTypes llvm_list_types;

#endif