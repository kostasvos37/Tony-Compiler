#include "ast.hpp"

llvm::LLVMContext AST::TheContext;
llvm::IRBuilder<> AST::Builder(TheContext);
llvm::GlobalVariable *AST::TheVars;
std::unique_ptr<llvm::Module> AST::TheModule;
std::map<std::string, llvm::AllocaInst*> AST::NamedValues;

llvm::Function *AST::TheMalloc;

llvm::Type *AST::i1;
llvm::Type *AST::i8;
llvm::Type *AST::i32;
llvm::Type *AST::i64;