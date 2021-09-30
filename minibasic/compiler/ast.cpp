#include "ast.hpp"

LLVMContext AST::TheContext;
IRBuilder<> AST::Builder(AST::TheContext);
std::unique_ptr<Module> AST::TheModule;
std::unique_ptr<legacy::FunctionPassManager> AST::TheFPM;

GlobalVariable *AST::TheVars;
GlobalVariable *AST::TheNL;
Function *AST::TheWriteInteger;
Function *AST::TheWriteString;
Function *AST::TheInit;
Function *AST::TheMalloc;

Type *AST::i8;
Type *AST::i32;
Type *AST::i64;
Type *AST::TheNodeTypePtr;