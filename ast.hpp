#ifndef __AST_HPP___
#define __AST_HPP___

#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include "symbol.hpp"
#include "runtime.hpp"
#include "type.hpp"
#include "error.hpp"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Value.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Utils.h>

void yyerror(const char *msg, ...);

bool check_type_equality(TonyType* type1, TonyType* type2);
bool is_nil_constant(TonyType *type);

class AST {
public:
  virtual ~AST() {}
  virtual void printOn(std::ostream &out) {};
  virtual void sem() {}; // NOTE: After we implement `sem()` for all the subclasses
                         // it would make sense to make this: `virtual void sem() = 0`.
  void llvm_compile_and_dump(bool optimize){
    // Initialize    
    TheModule = std::make_unique<llvm::Module>("tony program", TheContext);

    i1 = llvm::IntegerType::get(TheContext, 1);
    i8 = llvm::IntegerType::get(TheContext, 8);
    i32 = llvm::IntegerType::get(TheContext, 32);
    i64 = llvm::IntegerType::get(TheContext, 64);
    voidT = llvm::Type::getVoidTy(TheContext);

    // We'll use 'TheMalloc' to allocate memory blocks with LLVM.
    // This is needed currently for creating Tony lists.
    llvm::FunctionType *malloc_type = 
      llvm::FunctionType::get(llvm::PointerType::get(i8, 0),
                              {i32}, false);
    TheMalloc = 
      llvm::Function::Create(malloc_type, llvm::Function::ExternalLinkage,
                             "GC_malloc", TheModule.get());
    
    // Global Scope
    scopes.openRuntimeScope();
    initializeLibraryFunctions();
    scopes.openRuntimeScope();
    
    
    //Create "dummy main function" which calls the first program function
    llvm::FunctionType *main_type = llvm::FunctionType::get(i32, std::vector<llvm::Type *>{}, false);
    llvm::Function * Main = llvm::Function::Create(main_type, llvm::Function::ExternalLinkage, "main", TheModule.get());
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", Main);
 
    // Emit Program Code
    
    auto program = compile();
    auto *progFunc = TheModule->getFunction(program->getName());

    Builder.SetInsertPoint(BB);
    Builder.CreateCall(progFunc, {});
    Builder.CreateRet(c32(0));

    // Close Program Scope
    scopes.closeRuntimeScope();
    scopes.closeRuntimeScope();

    bool bad = llvm::verifyModule(*TheModule, &llvm::errs());
    if (bad){
      std::cerr << "The IR is bad!\n";
      TheModule->print(llvm::errs(), nullptr);
      std::exit(1);
    }

    if(optimize){
      optimizeModule();
    }

    std::error_code EC;

    llvm::raw_ostream *out2 = new llvm::raw_fd_ostream("out.ll", EC);
    TheModule->print(*out2, nullptr);
  }

  void optimizeModule(){

    TheFPM = std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());
    if (true) {
      TheFPM->add(llvm::createPromoteMemoryToRegisterPass());
      TheFPM->add(llvm::createInstructionCombiningPass());
      TheFPM->add(llvm::createReassociatePass());
      TheFPM->add(llvm::createGVNPass());
      TheFPM->add(llvm::createCFGSimplificationPass());
    }
    TheFPM->doInitialization();

    for (auto &F: *TheModule)
      TheFPM->run(F);

  }

  void initializeLibraryFunctions(){
    
    // puti (int n)
    llvm::FunctionType *writeIntegerType = llvm::FunctionType::get(voidT, std::vector<llvm::Type *>{i32}, false);
	  scopes.insertFunc("puti",  llvm::Function::Create(writeIntegerType, llvm::Function::ExternalLinkage, "writeInteger", TheModule.get()));

    // putc (char c)
    llvm::FunctionType *writeCharType = llvm::FunctionType::get(voidT, std::vector<llvm::Type *>{i8}, false);
	  scopes.insertFunc("putc",  llvm::Function::Create(writeCharType, llvm::Function::ExternalLinkage, "writeChar", TheModule.get()));

    // putb (bool b)
    llvm::FunctionType *writeBooleanType = llvm::FunctionType::get(voidT, std::vector<llvm::Type *>{i1}, false);
	  scopes.insertFunc("putb",  llvm::Function::Create(writeBooleanType, llvm::Function::ExternalLinkage, "writeBoolean", TheModule.get()));

    // puts (char [])
    llvm::FunctionType *writeStringType = llvm::FunctionType::get(voidT, {llvm::PointerType::get(i8,0)}, false);
    scopes.insertFunc("puts", llvm::Function::Create(writeStringType, llvm::Function::ExternalLinkage, "writeString", TheModule.get()));
    // int geti ()
    llvm::FunctionType *getiType = llvm::FunctionType::get(i32, std::vector<llvm::Type *>{}, false);
	  scopes.insertFunc("geti",  llvm::Function::Create(getiType, llvm::Function::ExternalLinkage, "readInteger", TheModule.get()));
    
    // char getc()
    llvm::FunctionType *getcType = llvm::FunctionType::get(i8, std::vector<llvm::Type *>{}, false);
	  scopes.insertFunc("getc",  llvm::Function::Create(getcType, llvm::Function::ExternalLinkage, "readChar", TheModule.get()));
    
    // bool getb ()
    llvm::FunctionType *getbType = llvm::FunctionType::get(i1, std::vector<llvm::Type *>{}, false);
	  scopes.insertFunc("getb",  llvm::Function::Create(getbType, llvm::Function::ExternalLinkage, "readBoolean", TheModule.get()));

    // char [] gets () TODO:FIX
    llvm::FunctionType *getsType = llvm::FunctionType::get(voidT, std::vector<llvm::Type *>{i32, i8->getPointerTo()}, false);
	  scopes.insertFunc("gets",  llvm::Function::Create(getsType, llvm::Function::ExternalLinkage, "readString", TheModule.get()));


    // int abs (int n)
    llvm::FunctionType *abstype = llvm::FunctionType::get(i32, std::vector<llvm::Type *>{i32}, false);
	  scopes.insertFunc("abs",  llvm::Function::Create(abstype, llvm::Function::ExternalLinkage, "abs", TheModule.get()));

    // int ord(char c)
    llvm::FunctionType *ordtype = llvm::FunctionType::get(i32, std::vector<llvm::Type *>{i8}, false);
	  scopes.insertFunc("ord",  llvm::Function::Create(ordtype, llvm::Function::ExternalLinkage, "ord", TheModule.get()));

    //char chr (int n)
    llvm::FunctionType *chrtype = llvm::FunctionType::get(i8, std::vector<llvm::Type *>{i32}, false);
	  scopes.insertFunc("chr",  llvm::Function::Create(chrtype, llvm::Function::ExternalLinkage, "chr", TheModule.get()));



    //TODO: int strlen (char [] s)
    llvm::FunctionType *strlenType = llvm::FunctionType::get(i32, std::vector<llvm::Type *>{i8->getPointerTo()}, false);
	  scopes.insertFunc("strlen",  llvm::Function::Create(strlenType, llvm::Function::ExternalLinkage, "strlen", TheModule.get()));

    //TODO: int strcmp (char[] s1, s2)
    //TODO: strcpy (char[] trg, src)
    //TODO: strcat (char[] trg, src)
  
  }
  
  virtual llvm::Value *compile() = 0;
  virtual void setLineno(int n){
    lineno = n;
  }
protected:

  int lineno;
  
  static llvm::LLVMContext TheContext;
  static llvm::IRBuilder<> Builder;
  static llvm::GlobalVariable *TheVars;
  static std::unique_ptr<llvm::Module> TheModule;
  static llvm::Function *TheMalloc;
  static std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM;

  static llvm::Type *i1;
  static llvm::Type *i8;
  static llvm::Type *i32;
  static llvm::Type *i64;
  static llvm::Type *voidT;

  static std::vector<RuntimeBlock *> blocks;
  static RuntimeScope scopes;
    
  static llvm::ConstantInt* c1(bool b) {
    if(b) return llvm::ConstantInt::getTrue(TheContext);
    else return llvm::ConstantInt::getFalse(TheContext);
    
  }
  static llvm::ConstantInt* c8(char c) {
    return llvm::ConstantInt::get(TheContext, llvm::APInt(8, c, true));
  }
  static llvm::ConstantInt* c32(int n) {
    return llvm::ConstantInt::get(TheContext, llvm::APInt(32, n, true));
  }

  /*
   * This function creates a pointer to the LLVM type that corresponds
   * to a TonyType. For the case of simple expressions, like integers,
   * LLVM already provides types (e.g. `i32`).
   * BUT for the case of more complex structures, like lists, we
   * must construct the corresponding LLVM type.
   */
  static llvm::Type* getOrCreateLLVMTypeFromTonyType(TonyType *t, PassMode mode = VAL) {
    
    //Initializing to avoid warning
    llvm::Type *retType = i32;
    switch(t->get_current_type()) {
      case TYPE_int:  retType = i32; break;
      case TYPE_bool: retType = i1;  break;
      case TYPE_char: retType = i8;  break;
      case TYPE_any:  retType = i32; break;
      case TYPE_void: retType = voidT;  break;
      case TYPE_list: {
        std::string hash = t->createHashKeyForType();
        llvm::Type* list_type = llvm_list_types.lookup(hash);

        // In this case, we haven't created a type for such a list yet.
        // So, we will create it and insert it in `llvm_list_types`.        
        if (list_type == nullptr) {
          llvm::StructType* node_type = llvm::StructType::create(TheContext, "nodetype");
          llvm::PointerType* pointer_to_node_type = llvm::PointerType::get(node_type, 0);

          // The value of a node can be a list too. So, we must construct its type
          // first by calling `getOrCreateLLVMTypeFromTonyType` recursively.
          llvm::Type* node_value_type = 
            getOrCreateLLVMTypeFromTonyType(t->get_nested_type());
          
          // A list node has type `node_type` and consists of:
          // - a value of type `node_value_type`
          // - a pointer to a list node of type `pointer_to_node_type`
          node_type->setBody({node_value_type, pointer_to_node_type});
          llvm_list_types.insert(hash, pointer_to_node_type);
          list_type = pointer_to_node_type;
        }
        retType = list_type; break;
      }
      case TYPE_array: {
        llvm::Type* element_type =
          getOrCreateLLVMTypeFromTonyType(t->get_nested_type());
        if (t->get_array_size() == 0) {
          llvm::PointerType* array_type =
            llvm::PointerType::get(element_type, 0);
          retType = array_type;
        } else {
        llvm::ArrayType* array_type =
          llvm::ArrayType::get(element_type, t->get_array_size());
        retType = array_type;
        }
        break;
      }
      default: yyerror("Type conversion not implemented yet");
    }

    if(mode == REF) {
      retType = retType->getPointerTo();
    }
    return retType;
  }

  // This should be simple, maybe more complex with lists or arrays
  llvm::Type *getLLVMRefType(llvm::Type *orgType){
    return orgType->getPointerTo();
  }

  static llvm::AllocaInst *CreateEntryBlockAlloca (llvm::Function *TheFunction, const std::string &VarName, llvm::Type* Ty){
    llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(Ty, 0, VarName.c_str());
  }


};

inline std::ostream& operator<< (std::ostream &out, AST &t) {
  t.printOn(out);
  return out;
}


class Expr: public AST {
public:
  bool type_check(TonyType* t) {
    sem();
    return check_type_equality(t, type);
  }
  bool type_check(TypeBlock t) {
    /*
     * This is just an overloading of the method above.
     * It is used for simpler cases.
    */    
    sem();
    return (t == type->get_current_type() && type->get_nested_type() == nullptr);
  }
  TonyType* get_type() {
    /* 
      We want to be able to access `type` from a pointer to an `Expr`.
      But `type` is protected, so we need this `public` method.
    */
    return type;
  }

  void setLLVMType(llvm::Type* t) {
    LLVMType = t;
  }

protected:
  TonyType* type;
  llvm::Type* LLVMType;
};

class Stmt: public AST {
public:
};


class Atom: public Expr { 
public:
  virtual bool isLvalue() {return false;}
  virtual std::string getName() = 0;
  virtual void setPassByValue(bool b) {
    pass_by_value=b;
  }
protected:
  bool pass_by_value;
};

class Simple: public Stmt {  
};

class Id: public Atom {
public:
  Id(std::string v): var(v) {}
  ~Id() {}
  virtual void printOn(std::ostream &out) override;

  void set_type(TonyType* t){
    type = t;
  }

  void insertIntoScope(TableType l){
    st.insert(var, type, l);
  }

  void insertIntoParentScope(TableType l){
    st.insertIntoParentScope(var, type, l);
  }

  virtual std::string getName() override{
    return var;
  }

  virtual void sem() override; 

  virtual llvm::Value *compile() override{
    if(!blocks.back()->isRef(var)){
      // By value variable
      llvm::Value *v = blocks.back()->getVal(var);

      return Builder.CreateLoad(v);
    } else{
      // By reference variable
      auto *addr = Builder.CreateLoad(blocks.back()->getAddr(var));
      return Builder.CreateLoad(addr);
    }
  } 

  virtual bool isLvalue() override{
    return true;
  }
private:
  std::string var;
};

class ArrayElement: public Atom {
public:
  ArrayElement(Atom *a, Expr *e): atom(a), expr(e) {
    pass_by_value=true;
  }
  ~ArrayElement() {delete atom; delete expr;}
  virtual void printOn(std::ostream &out) override;
  virtual void sem() override;

  virtual bool isLvalue() override{
    return true;
  }

  virtual std::string getName() override{
    return atom->getName();
  }

  virtual llvm::Value *compile() override{
    llvm::Value* array_index = expr->compile();
    llvm::Value *v, *array;
    if(dynamic_cast<Id*>(atom) == nullptr) {
      // In this case, the atom is not an Id (e.g: a[5]), instead
      // it may be another ArrayElement (e.g. a[4][2]) or a function
      // call (e.g. f(arr)[0]). So, we compile the atom to get the
      // array.
      array = atom->compile();
    } else if(blocks.back()->isRef(getName())) {
      // In this case, the atom is an Id (e.g: a[0]), and it is a
      // function parameter that is passed by reference (e.g. ref int[] a).
      v = Builder.CreateLoad(blocks.back()->getAddr(getName()));
      array = Builder.CreateLoad(v, getName().c_str());
    } else {
      // In this case, the atom is an Id (e.g: a[0]), and it is a
      // function parameter that is passed by value.
      v = blocks.back()->getVal(getName());
      array = Builder.CreateLoad(v, getName().c_str());
    }

    llvm::Value* elem_ptr = Builder.CreateGEP(array, array_index);
    if (pass_by_value) {
      // In this case, the ArrayElement is NOT used on the left side
      // of an assignment, but on the right. So, we return the loaded
      // value of the element.
      return Builder.CreateLoad(elem_ptr, "elem");
    }
    return elem_ptr;
  } 
private:
  Atom *atom;
  Expr *expr;
};

class StringLiteral: public Atom {
public:
  StringLiteral(std::string str) {
    int len = str.length();
    strlit = str;
    int i = 0; // Iterates over `str`.
    int j = 0; // Iterates over `strlit`.
    
    // We validate the escape characters (e.g: '\n', \0' etc...) in the string.
    // Initially they are parsed as two separate characters, so we must
    // squeeze them into a single character.
    while(i < len) {
      if(str[i] == '\\') {
        if(i == len-1) {
          yyerror("Invalid backslash at the end of string literal.");
        }

        // In this case, we've caught an escape character.
        switch(str[i+1]) {
          case 'n':  strlit[j] = '\n'; i+=2; break;
          case 't':  strlit[j] = '\t'; i+=2; break;
          case 'r':  strlit[j] = '\r'; i+=2; break;
          case '0':  strlit[j] = '\0'; i+=2; break;
          case '\\': strlit[j] = '\\'; i+=2; break;
          case '\'': strlit[j] = '\''; i+=2; break;
          case '\"': strlit[j] = '\"'; i+=2; break;
          case 'x':  strlit[j] = char(stoi(str.substr(i+2, 2), 0, 16)); i+=4; break;
        default:
          yyerror("Invalid escape character in string literal.");
        }
      } else {
        // In this case, we've caught a regular character.
        strlit[j] = str[i];
        i++;
      }
      j++;
    }

    // We throw away the trailing unused characters.
    strlit = strlit.substr(0, j);
  }
  ~StringLiteral () {}
  virtual void printOn(std::ostream &out) override;
  virtual void sem() override;

  virtual bool isLvalue() override{
    return false;
  }

  virtual std::string getName() override{
    return strlit;
  }

  virtual llvm::Value *compile() override{
    llvm::Value* p =
      Builder.CreateCall(TheMalloc, c32(strlit.length()+1), "strlitaddr");
    p = Builder.CreateBitCast(p, getOrCreateLLVMTypeFromTonyType(type));
    llvm::Value* char_ptr;
    for (std::string::size_type i = 0; i < strlit.length(); i++) {
        char_ptr = Builder.CreateGEP(p, c32(i));
        Builder.CreateStore(c8(strlit[i]), char_ptr);
    }
    char_ptr = Builder.CreateGEP(p, c32(strlit.length()));
    Builder.CreateStore(c8('\0'), char_ptr);
    return p;
  } 
private:
  std::string strlit;
};

class CharConst: public Expr {
public:
  CharConst(unsigned char c): char_const(c) {}
  ~CharConst() {}
  virtual void printOn(std::ostream &out) override;
  virtual void sem() override;

  virtual llvm::Value *compile() override{
    return c8(char_const);
  } 
private:
  unsigned char char_const;
};

class IntConst: public Expr {
public:
  IntConst(int n): num(n) {}
  ~IntConst() {}
  virtual void printOn(std::ostream &out) override;
  virtual void sem() override;

  virtual llvm::Value *compile () override{
    return c32(num);
  } 
private:
  int num;
};

class New: public Expr {
public:
  New(TonyType *t, Expr *right): type_of_elems(t), expr(right) {}
  ~New() {delete type_of_elems; delete expr;}
  virtual void printOn(std::ostream &out) override;

  virtual void sem() override;

  virtual llvm::Value *compile() override{
    llvm::Value* e = expr->compile();
    llvm::Value* size =
      Builder.CreateMul(e, c32(type_of_elems->get_data_size_of_type()),
                        "sizeformalloc");
    llvm::Value* p =
      Builder.CreateCall(TheMalloc, {size}, "arrayaddr");
    return p;
  } 
private:
  TonyType *type_of_elems;
  Expr *expr;
};

/*
 *  This is a constant value (check Tony language description).
 *  It is equal to an empty list and has type: list[t], where `t` can be any type.
 */
class Nil: public Expr {
public:
  Nil() {}
  ~Nil() {}
  virtual void printOn(std::ostream &out) override;
  virtual void sem() override;
  virtual llvm::Value *compile() override{
    return llvm::ConstantPointerNull::get(llvm::Type::getInt32Ty(TheContext)->getPointerTo());
  }
};

class Boolean: public Expr {
public:
  Boolean(std::string b1): boolean_value(b1) {
    if (b1=="true") b=true;
    else b=false;
  }
  ~Boolean() {}
  virtual void printOn(std::ostream &out) override;
  virtual void sem() override;

  virtual llvm::Value *compile() override{
    return c1(b);
  } 
private:
  std::string boolean_value;
  bool b;
};

class BinOp: public Expr {
public:
  BinOp(Expr *l, std::string o, Expr *r): left(l), op(o), right(r) {}
  ~BinOp() { delete left; delete right; }
  virtual void printOn(std::ostream &out) override;
  virtual void sem() override;

  virtual llvm::Value *compile() override{
    
    llvm::Value *l = left->compile();
    llvm::Value *r = right->compile();
    
    if(op ==  "+")          return Builder.CreateAdd(l, r, "addtmp");
    else if(op ==  "-")     return Builder.CreateSub(l, r, "subtmp");
    else if(op ==  "*")     return Builder.CreateMul(l, r, "multmp");
    else if(op ==  "/")     return Builder.CreateSDiv(l, r, "divtmp");
    else if(op ==  "mod")   return Builder.CreateSRem(l, r, "remtmp");
    else if(op ==  "=")     return Builder.CreateICmpEQ(l,r, "eqtmp");
    else if(op ==  "<>")    return Builder.CreateICmpNE(l,r, "neqtmp");
    else if(op ==  "<")     return Builder.CreateICmpSLT(l,r, "slttmp");
    else if(op ==  ">")     return Builder.CreateICmpSGT(l,r, "sgttmp");
    else if(op ==  "<=")    return Builder.CreateICmpSLE(l,r, "sletmp");
    else if(op ==  ">=")    return Builder.CreateICmpSGE(l,r, "sgetmp");
    else if(op ==  "and")   return Builder.CreateAnd(l,r, "andtmp");
    else if(op ==  "or")    return Builder.CreateOr(l,r, "ortmp");
    else if(op ==  "#") {

      LLVMType = getOrCreateLLVMTypeFromTonyType(type);
      llvm::Type* LLVMTypeOfElement =
        getOrCreateLLVMTypeFromTonyType(type->get_nested_type());

      int size = left->get_type()->get_data_size_of_type();
      llvm::Value *p = Builder.CreateCall(TheMalloc, {c32(size)}, "newtmp");
      llvm::Value *n = Builder.CreateBitCast(p, LLVMType, "nodetmp");
      llvm::Value *h = Builder.CreateStructGEP(n, 0, "headptr");
      l = Builder.CreateBitCast(l, LLVMTypeOfElement);
      Builder.CreateStore(l, h);
      llvm::Value *t = Builder.CreateStructGEP(n, 1, "tailptr");
      r = Builder.CreateBitCast(r, LLVMType);
      Builder.CreateStore(r, t);
      return n;
    }
    else                    yyerror("Operation not supported yet");
    return nullptr;
  }
private:
  Expr *left;
  std::string op;
  Expr *right;
};

class UnOp: public Expr {
public:
  UnOp(std::string(o), Expr *r): op(o), right(r) {}
  ~UnOp() { delete right; }
  virtual void printOn(std::ostream &out) override;
  virtual void sem() override;

  virtual llvm::Value *compile() override{
    llvm::Value *r = right->compile();

    if (op == "+")     return r;
    if (op == "-")     return Builder.CreateNeg(r);
    if (op == "not")   return Builder.CreateNot(r);
    if (op == "head") {
      // We want to get a pointer to the head, so we must typecast.
			r = Builder.CreateBitCast(
        r, getOrCreateLLVMTypeFromTonyType(right->get_type()));
			
      // We get a pointer to the value of the head.
      r = Builder.CreateStructGEP(r, 0);

      // We load the value.
			r = Builder.CreateLoad(r);
      return r;
    }
    if (op == "tail") {

      // We want to get a pointer to the head, so we must typecast.
			r = Builder.CreateBitCast(
        r, getOrCreateLLVMTypeFromTonyType(right->get_type()));
			
      // We get a pointer to the tail.
      r = Builder.CreateStructGEP(r, 1);

      // We load the value.
			r = Builder.CreateLoad(r);
      return r;
    }
    if (op == "nil?") {
      llvm::Value *nil_type = llvm::ConstantPointerNull::get(static_cast<llvm::PointerType *>(r->getType()));
			return Builder.CreateICmpEQ(r, nil_type);
    }
    return nullptr;
  } 
private:
  std::string op;
  Expr *right;
};

class VarList: public AST {
public:
  VarList() {}
  ~VarList() {
    for (Id * i : ids) delete i;
  }

  void append(Id * id) {
    ids.push_back(id);
  }
  
  void reverse() {
    std::reverse(ids.begin(), ids.end());
  }
  
  /*
   This is a method that is called in `parser.y` to set the type of the
   `VarList`, after the `ids` vector is filled with all the variables.
   */
  void set_type(TonyType* t) {
    type = t;
  }

  virtual void printOn(std::ostream &out) override;
  virtual void sem() override;

  virtual llvm::Value *compile() override{
    llvm::Type* t = getOrCreateLLVMTypeFromTonyType(type);
    for (Id * id: ids) {
      // TODO: PASSMODE
      llvm::AllocaInst* alloca = Builder.CreateAlloca(t, 0, id->getName());
      blocks.back()->addVar(id->getName(), t, type->getPassMode());
      blocks.back()->addAddr(id->getName(), alloca);
      blocks.back()->addVal(id->getName(), alloca);
    }
    return nullptr;
  }

  std::pair<TonyType*, int> getArgs(){
    std::pair<TonyType*, int> p1;
    p1.first = type;
    p1.second = (int) ids.size();
    return p1;
  }

  std::vector<std::string> getNames(){
    std::vector<std::string> ret;
    for (Id * i: ids) ret.push_back(i->getName());
    return ret;
  }

  void setIsRef(bool b){
    isRef = b;
  }

protected:
  std::vector<Id *> ids;
  TonyType* type;
  bool isRef;
};

class Formal: public AST {
public:
  Formal(VarList* v, bool i): var_list(v), is_ref(i) {}
  ~Formal() {delete var_list;}
  virtual void printOn(std::ostream &out) override;
  
  virtual void sem() override;

  std::pair<TonyType*, int> getArgs() {
    return var_list->getArgs();
  }

  std::vector<std::string> getNames() {
    return var_list->getNames();
  }

  // Not implemented yet
  virtual llvm::Value *compile() override{
    return nullptr;
  }
private:
  VarList* var_list;
  bool is_ref;
};

class FormalList: public AST {
public:
  FormalList() {}
  ~FormalList() {
    for (Formal * f : formals) delete f;
  }

  void append(Formal * f) {
    formals.push_back(f);
  }

  void reverse(){
    std::reverse(formals.begin(), formals.end());
  }

  virtual void printOn(std::ostream &out) override;
  virtual void sem() override;

  // Not implemented yet
  virtual llvm::Value *compile() override{
    return nullptr;
  } 

  //This is fine, don't need pointers
  std::vector<TonyType *> getArgs(){
    std::vector<TonyType *> ret;
    for (Formal *f: formals){
      std::pair<TonyType*, int> p1 = f->getArgs(); 
      for (int i=0; i < p1.second; i++){
        ret.push_back(p1.first);
      }
    }

    return ret;
  }

  std::vector<std::string> getNames(){
    std::vector<std::string> ret;
    for (Formal *f: formals){
      std::vector<std::string> p1 = f->getNames(); 
      for (int i=0; i < p1.size(); i++){
        ret.push_back(p1[i]);
      }
    }

    return ret;
  }
private:
  std::vector<Formal *> formals;
};

class Header: public AST {
public:
  Header(TonyType *t, Id *name, FormalList *f): type(t), formals(f), id(name), isTyped(true) {}
  Header(Id *name, FormalList *f): formals(f), id(name), isTyped(false) {type = new TonyType(TYPE_void, nullptr);}
  ~Header(){ delete formals; delete id;}
  virtual void sem() override;
  virtual void printOn(std::ostream &out) override;

  TonyType *getType() {return type;}

  // To handle declarations and definitions
  // The way it is structured, the function adds its own header to above function's scope
  // Declarations have headers too, but need to insert on same scope, hence 2 different sem() functions
  void semHeaderDecl();
  void semHeaderDef();

  bool getIsTyped(){
    return isTyped;
  }

  std::string getName(){
    return id->getName();
  }

  virtual llvm::Function *compile() override{
    
    llvm::Twine name = llvm::Twine(id->getName());
    std::vector<TonyType *> args;
    std::vector<std::string> names;
    if (formals){
      args = formals->getArgs();
      names = formals->getNames();
    }

    std::vector<llvm::Type *> ArgumentTypes;
    for (auto i: args){
      ArgumentTypes.push_back(getOrCreateLLVMTypeFromTonyType(i));
    }
    llvm::FunctionType *FT =
      llvm::FunctionType::get(getOrCreateLLVMTypeFromTonyType(type),
                              ArgumentTypes, false);
    llvm::Function *F = llvm::Function::Create(FT,
      llvm::Function::ExternalLinkage, name, TheModule.get());
    
    unsigned i = 0;
    for (auto &Arg: F->args()) Arg.setName(names[i++]);


    return F;
  } 

  std::vector<TonyType *>   getArgs(){
    if(formals)
      return formals->getArgs();
    else
      return std::vector<TonyType *> ();
  }
  std::vector<std::string> getNames(){
    if(formals)
      return formals->getNames();
    else
      return std::vector<std::string> ();
  }


private:
  TonyType *type;
  FormalList *formals;
  Id *id;
  bool isTyped;
};

class Return: public Stmt{
public:
  Return(Expr* e): ret_expr(e) {}
  ~Return() {delete ret_expr;}
  virtual void printOn(std::ostream &out) override;

  virtual void sem() override;

  virtual llvm::Value *compile() override{

    llvm::Value* v = ret_expr->compile();

    if (is_nil_constant(ret_expr->get_type())) {
      // If `nil` is returned, we must change its LLVM type
      // (null pointer to an i32) to the return type of the
      // function.
      v = Builder.CreateBitCast(v, blocks.back()->getFun()->getReturnType());
    }

    return Builder.CreateRet(v);
  }
private:
  Expr* ret_expr;
};

class Exit: public Stmt{
public:
  Exit() {}
  virtual void printOn(std::ostream &out) override;

  virtual void sem() override;

  llvm::Value* compile() override{return Builder.CreateRetVoid();}
};

class StmtBody: public AST {
public:
  StmtBody(): stmts(), has_return(false), has_exit(false) {}
  ~StmtBody() {for (Stmt *s : stmts) delete s;}
  
  bool hasReturnStmt() {
    return has_return;
  }

  bool hasExitStmt() {
    return has_exit;
  }

  void append(Stmt* stmt) {
    stmts.push_back(stmt);
  }

  void reverse(){
    std::reverse(stmts.begin(), stmts.end());
  }

  virtual void printOn(std::ostream &out) override;
  virtual void sem() override;

  virtual llvm::Value *compile() override{
    for (Stmt *s : stmts) {
      s->compile();
      if (dynamic_cast<Return*>(s) != nullptr) {
        // In this case, we've reached a `return` statement.
        // So, we don't compile the remaining statements in the
        // statement body.
        has_return = true;
        break;
      }
      if (dynamic_cast<Exit*>(s) != nullptr) {
        // In this case, we've reached a `return` statement.
        // So, we don't compile the remaining statements in the
        // statement body.
        has_exit = true;
        break;
      }
    }
    return nullptr;
  }
  
private:
  std::vector<Stmt*> stmts;
  bool has_return, has_exit;
};

class Assign: public Simple {
public:
  Assign(Atom *a, Expr *e): atom(a), expr(e) {}
  ~Assign() {delete atom; delete expr;}
  virtual void printOn(std::ostream &out) override;
  virtual void sem() override;

  llvm::Value* compile() override{
    llvm::Type*  LLVMType = getOrCreateLLVMTypeFromTonyType(atom->get_type());
    llvm::Value* value = expr->compile();
    llvm::Value* variable;

    // As opposed to variables, the address of an array ELEMENT cannot be
    // known beforehand (by looking at the RuntimeTable). So, we run
    // `ArrayElement.compile()` instead of just doing a lookup in the table.
    if (dynamic_cast<ArrayElement*>(atom) != nullptr) {

      // This will inform `ArrayElement.compile()` to return the address of
      // the element, not the actual value inside the element. This is
      // because we want to STORE the value of the right expression into
      // this element.
      atom->setPassByValue(false);
      variable = atom->compile();
      value = Builder.CreateBitCast(value, LLVMType);
      Builder.CreateStore(value, variable);
    } else {
      // This is the case for regular variables.
      if(blocks.back()->isRef(atom->getName())) {
        auto addr = Builder.CreateLoad(blocks.back()->getAddr(atom->getName()));
        value = Builder.CreateBitCast(value, LLVMType);
        Builder.CreateStore(value, addr);
      } else {
        value = Builder.CreateBitCast(value, LLVMType);
        Builder.CreateStore(value, blocks.back()->getVal(atom->getName()));
      }
    }

    return nullptr;
  } 
private:
  Atom *atom;
  Expr *expr;
};

class Skip: public Simple {
public:
  Skip() {}
  ~Skip() {}
  
  virtual void printOn(std::ostream &out) override;

  virtual llvm::Value *compile() override{
    return nullptr;
  } 

  virtual void sem() override;
};


/*
 * This class represents Tony's `if`, `elsif` and `else` statements, not just `if`.
 */
class If: public Stmt {
public:
  If(Expr* if_condition, StmtBody* if_stmt_body, If* next):
    condition(if_condition) ,stmt_body(if_stmt_body), next_if(next) {}
  ~If() {
    delete condition;
    delete stmt_body; 
    if(next_if != nullptr) delete next_if;
  }
  virtual StmtBody* getStmtBody() {
    return stmt_body;
  }

  virtual bool isElse() {
    return condition == nullptr;
  }

  virtual void printOn(std::ostream &out) override;

  virtual void sem() override;

  // TODO: Currently we create a `MergeBB` for each `if`/`elisf`/`else`
  // statement. This works but it isn't correct. We must have only one
  // `MergeBB` for the whole `if`-`then`-`else` statement.
  llvm::Value* compile() override{
    // IMPORTANT: We don't create a new basic block for this statement yet.
    // The condition check for an `if` statement remains in the same
    // basic block as statements before the `if`. BUT, later, we will start
    // a new basic block for the statement body under `then`.
    if (condition == nullptr) {
      // In this case, we are in an `else` statement (no condition).
      if (stmt_body != nullptr) {
        stmt_body->compile();
      }
      return nullptr;
    }

    llvm::Value* cond = condition->compile();
    // QUESTION: Can this ever be true?
    if(!cond) return nullptr;
    // We convert the condition to an LLVM bool
    cond = Builder.CreateICmpNE(cond, c1(0), "ifcond");
    // This is the function in which we're in. We will need this to create
    // basic blocks later on, for `then`, `else` etc... 
    llvm::Function* TheFunction = Builder.GetInsertBlock()->getParent();
    // We create the usual basic blocks (BB)
    llvm::BasicBlock* then_bb  = llvm::BasicBlock::Create(TheContext, "then", TheFunction);
    llvm::BasicBlock* else_bb  = llvm::BasicBlock::Create(TheContext, "else");

    // This is the basic block that all the previous blocks (for `then` and `else`)
    // will jump to, after their execution is finished. This way, the execution
    // will move on past the `if-then-else` statement.
    llvm::BasicBlock* merge_bb = llvm::BasicBlock::Create(TheContext, "exitfromif");

    if(next_if != nullptr) {
      Builder.CreateCondBr(cond, then_bb, else_bb);
    } else {
      Builder.CreateCondBr(cond, then_bb, merge_bb);
    }

    // This is the `then` statement. We start a new basic block.
    Builder.SetInsertPoint(then_bb);
    stmt_body->compile();
    // Basic blocks in LLVM end with either a `ret` or a `br`.
    if(!stmt_body->hasReturnStmt() && !stmt_body->hasExitStmt()) {
      Builder.CreateBr(merge_bb);
    }
    
    if(next_if != nullptr) {
      // Append the basic block for `else` to the list of basic blocks
      // of this function.
      TheFunction->getBasicBlockList().push_back(else_bb);
      // This is the `else` or `elsif` statement. We start a new basic block.
      Builder.SetInsertPoint(else_bb);
      next_if->compile();
      // Next `Ifs` must jump to the current merge block, after they're done.
      if(!next_if->isElse() ||
         (!next_if->getStmtBody()->hasReturnStmt() && !next_if->getStmtBody()->hasExitStmt())) {
        Builder.CreateBr(merge_bb);
      }
    }
    
    // Append the basic block that comes after the whole `if-then-else`
    // statement to the list of basic blocks of this current function.
    TheFunction->getBasicBlockList().push_back(merge_bb);
    // The execution of the program continues in a new basic block.
    Builder.SetInsertPoint(merge_bb);
    return nullptr;
  }


private:
  Expr*     condition;
  StmtBody* stmt_body;
  If*       next_if;
};

class SimpleList: public AST {
public:
  SimpleList(): simples() {}
  ~SimpleList() { for (Simple *s : simples) delete s; }

  void append(Simple *s) { simples.push_back(s); }

  void reverse(){
    std::reverse(simples.begin(), simples.end());
  }

  virtual void printOn(std::ostream &out) override;
  std::vector<Simple *> get_simples_list(){
    return simples;
  }

  virtual void sem() override;

  virtual llvm::Value *compile() override{
    for (auto s: simples){
      s->compile();
    }
    return nullptr;
  } 
private:
  std::vector<Simple *> simples;
};

class For: public Stmt {
public:
  For(SimpleList *sl1, Expr *e, SimpleList *sl2, StmtBody *sb):
    initializations(sl1), condition(e), steps(sl2), stmt_body(sb) {}
  ~For() {
    delete initializations;
    delete condition;
    delete steps;
    delete stmt_body;
  }

  virtual void printOn(std::ostream &out) override;

  virtual void sem() override;

  virtual llvm::Value *compile() override{
    initializations->compile();

    // Creating new BB for header after current block
    llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();
    llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(TheContext, "loop", TheFunction);
    llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(TheContext, "afterloop");

    //Check condition before starting loop
    llvm::Value *cond = condition->compile();
    cond = Builder.CreateICmpNE(cond, c1(0), "loopcond");    
    Builder.CreateCondBr(cond, LoopBB, AfterBB);


    // LoopBB : compiling steps and checking condition  
    Builder.SetInsertPoint(LoopBB);
    stmt_body->compile();
    if(!stmt_body->hasReturnStmt() && !stmt_body->hasExitStmt()) {
      steps->compile();
      cond = condition->compile();
      cond = Builder.CreateICmpNE(cond, c1(0), "loopcond");    
      Builder.CreateCondBr(cond, LoopBB, AfterBB);
    }

    //Emit Merge Block
    TheFunction->getBasicBlockList().push_back(AfterBB);
    Builder.SetInsertPoint(AfterBB);
    
    return nullptr;
  } 

private:
  SimpleList *initializations;
  Expr *condition;
  SimpleList *steps;
  StmtBody *stmt_body;
};

class ExprList: public AST {
public:
  ExprList(): expressions() {}
  ~ExprList() { for (Expr *e : expressions) delete e; }

  void append(Expr *e) { expressions.push_back(e); };

  void reverse(){
    std::reverse(expressions.begin(), expressions.end());
  }

  virtual void printOn(std::ostream &out) override;

  std::vector<Expr*> get_expr_list(){
    return expressions;
  }

  virtual void sem() override;

  // This DOESN'T need to be implemented.
  virtual llvm::Value* compile() override{
    return nullptr;
  }

private:
  std::vector<Expr*> expressions;
};

class FunctionCall: public Simple, public Atom {
public:
  FunctionCall(Id *n): name(n), hasParams(false) {}
  FunctionCall(Id *n, ExprList *el): name(n), params(el), hasParams(true) {}
  ~FunctionCall() {delete name; if (hasParams) delete params;}
  virtual void printOn(std::ostream &out) override;
  virtual void sem() override;

  virtual bool isLvalue() override{
    return false;
  }
  
  void setLineno(int n) override{
    lineno = n;
  }

  llvm::Value* compile() override{
    llvm::Function* llvm_function = scopes.getFun(name->getName());
    std::vector<llvm::Value*> compiled_params;
    std::vector<Expr*> parameter_exprs;
    if (hasParams) {
      parameter_exprs = params->get_expr_list();
    }   

    std::vector<TonyType *> functionArgs = name->get_type()->get_function_args();
    std::map<std::string, TonyType*> previousScopeArgs = name->get_type()->getPreviousScopeArgs();

    llvm::Value* v;
    int index = 0;
    auto arg = llvm_function->arg_begin();

    for (auto par: functionArgs) {
      PassMode p = par->getPassMode();
      if(p == VAL) {
        // In this case, the input argument 'arg' is passed BY VALUE in the
        // function.
        v = parameter_exprs[index]->compile();
        if (is_nil_constant(parameter_exprs[index]->get_type())) {
          // If `nil` is passed as an input parameter, we must change
          // its LLVM type (null pointer to an i32) to the type of the
          // corresponding parameter in the function signature.
          v = Builder.CreateBitCast(v, arg->getType());
        }       
      } else {
        // In this case, the input argument 'arg' is passed BY REFERENCE in the
        // function.
        ArrayElement* arr_elem = dynamic_cast<ArrayElement*>(parameter_exprs[index]);
        if (arr_elem != nullptr) {
          // Special case, if 'arg' is an array element, e.g: a[0].
          arr_elem->setPassByValue(false);
          v = arr_elem->compile();
        } else {
          // General case, where 'arg' is a variable
          auto var = dynamic_cast<Id*> (parameter_exprs[index]);
          if(var != nullptr) {
            if(blocks.back()->isRef(var->getName())) {
              v = Builder.CreateLoad(blocks.back()->getAddr(var->getName()));
            } else {
              v = blocks.back()->getVal(var->getName());
            }
          } else {
            v = parameter_exprs[index]->compile();
          }
        }
      }
      compiled_params.push_back(v);
      arg++;
      index++;
    }

    // Iterating through previous Scope args (all references)
    for (auto par: previousScopeArgs) {
      if(blocks.back()->isRef(par.first)) {
        v = Builder.CreateLoad(blocks.back()->getAddr(par.first));
      } else {
        v =  blocks.back()->getVal(par.first);
      }
      compiled_params.push_back(v);
      arg++;
      index++;
    }

    return Builder.CreateCall(llvm_function, compiled_params);
  }

  virtual std::string getName() override{
    return name->getName();
  }

private:
  Id *name;
  ExprList *params;
  bool hasParams;
  int lineno;
};

class FunctionDeclaration: public AST {
public:
  FunctionDeclaration(Header *hd): header(hd){}
  ~FunctionDeclaration() {delete header;}
  virtual void printOn(std::ostream &out) override;

  virtual void sem() override;

  // Not implemented yet
  virtual llvm::Value *compile() override{
    RuntimeBlock* newBlock = new RuntimeBlock();
    blocks.push_back(newBlock);

    std::vector<TonyType*> argTypes = header->getArgs();
    std::vector<std::string> argNames = header->getNames();
    llvm::Type* argLLVMType;
    for(int i=0; i<argTypes.size(); i++) {
      argLLVMType =
        getOrCreateLLVMTypeFromTonyType(argTypes[i], argTypes[i]->getPassMode());
      blocks.back()->addArg(argNames[i], argLLVMType, argTypes[i]->getPassMode());
    }

    //Getting previous scope vars, only gets strings which are not already included in function parameters
    std::map<std::string, TonyType*> previous = functionType->getPreviousScopeArgs();
    for(auto it:previous){
      std::string varname = it.first;
      // First checking if it was already defined as a new function parameter
      if(blocks.back()->containsVar(varname)) continue;

      // Translating type and inserting as a REF parameter
      llvm::Type *translated = getOrCreateLLVMTypeFromTonyType(it.second, REF);
      blocks.back()->addArg(varname, translated, REF);
      argNames.push_back(varname);
      argTypes.push_back(it.second);
    }

    llvm::FunctionType *FT =
      llvm::FunctionType::get(getOrCreateLLVMTypeFromTonyType(header->getType(), header->getType()->getPassMode()),
                              blocks.back()->getArgs(), false);

    llvm::Function *Fun = llvm::Function::Create(FT,llvm::Function::ExternalLinkage, header->getName(), TheModule.get());
    blocks.back()->setFun(Fun);
    scopes.insertFunc(header->getName(), Fun);
    blocks.pop_back();
    return Fun;  
  } 

private:
  Header *header;
  TonyType *functionType;
};

class FunctionDefinition: public AST {
public:
  FunctionDefinition(): header(), body(){}
  ~FunctionDefinition() {
    delete header; delete body;
    for (AST *a : local_definitions) delete a;
  }

  void initFunctions();
  std::string getName(){
    return header->getName();
  }

  void append(AST* a) {
    local_definitions.push_back(a);
  }

  void merge(Header *hd, StmtBody *st) {

    header = hd;
    body = st;
  }

  void reverse(){
    std::reverse(local_definitions.begin(), local_definitions.end());
  }

  virtual void printOn(std::ostream &out) override;

  virtual void sem() override;

  void setIsMain(){
    isMain = true;
  }

  llvm::Value* compile() override{


    RuntimeBlock* newBlock = new RuntimeBlock();
    blocks.push_back(newBlock);


    llvm::Function *Fun = scopes.getFunCurrentScope(header->getName());
    bool isDeclared = (Fun != nullptr);

    

    
    std::vector<TonyType*> argTypes = header->getArgs();
    std::vector<std::string> argNames = header->getNames();
    llvm::Type* argLLVMType;
    for(int i=0; i<argTypes.size(); i++) {
      argLLVMType =
        getOrCreateLLVMTypeFromTonyType(argTypes[i], argTypes[i]->getPassMode());
      blocks.back()->addArg(argNames[i], argLLVMType, argTypes[i]->getPassMode());
    }

    //Getting previous scope vars, only gets strings which are not already included in function parameters
    std::map<std::string, TonyType*> previous = functionType->getPreviousScopeArgs();
    for(auto it:previous){
      std::string varname = it.first;
      // First checking if it was already defined as a new function parameter
      if(blocks.back()->containsVar(varname)) continue;

      // Translating type and inserting as a REF parameter
      llvm::Type *translated = getOrCreateLLVMTypeFromTonyType(it.second, REF);
      blocks.back()->addArg(varname, translated, REF);
      argNames.push_back(varname);
      argTypes.push_back(it.second);
    }

    // TODO: Here i should first check if func is declared


    if(!isDeclared){
      llvm::FunctionType *FT =
        llvm::FunctionType::get(getOrCreateLLVMTypeFromTonyType(header->getType(), header->getType()->getPassMode()),
                                blocks.back()->getArgs(), false);

      Fun = llvm::Function::Create(FT,llvm::Function::ExternalLinkage, header->getName(), TheModule.get());
    }



    blocks.back()->setFun(Fun);

    // This will overwrite declared function
    scopes.insertFunc(header->getName(), Fun);
    scopes.openRuntimeScope();
        
    int index = 0;
    for(auto &Arg: Fun->args()){
      Arg.setName(argNames[index++]);
    }
  
    // CREATE Basic Block
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", Fun);
    Builder.SetInsertPoint(BB);    
    blocks.back()->setBlock(BB);

    // Insert Parameters
    for (auto &arg: Fun->args()) {
      llvm::AllocaInst * Alloca = CreateEntryBlockAlloca(Fun, arg.getName().str(), arg.getType());
      if(blocks.back()->isRef(std::string(arg.getName()))) {
        blocks.back()->addAddr(std::string(arg.getName()), Alloca);
      } else {
        blocks.back()->addVal(std::string(arg.getName()), Alloca);
      }
      Builder.CreateStore(&arg, Alloca);
    }

    // Compile other definitions
    for(AST *a: local_definitions) a->compile();
    Builder.SetInsertPoint(BB);

    body->compile();
    
    if (Fun->getReturnType()->isVoidTy()) {
      if (!body->hasExitStmt()) {
			  Builder.CreateRetVoid();
      }
    }
		else { 
			if (!body->hasReturnStmt()) {
        // Return a 'dummy' value for the case that the function
        // has no return statement.
				if (Fun->getReturnType()->isIntegerTy(32))
					Builder.CreateRet(c32(0));
				else if(Fun->getReturnType()->isIntegerTy(8))
					Builder.CreateRet(c8(0));
				else if(Fun->getReturnType()->isIntegerTy(1))
					Builder.CreateRet(c1(0));
        else {
          llvm::Type* ret_type = getOrCreateLLVMTypeFromTonyType(header->getType());
					llvm::ConstantPointerNull* ret_val =
            llvm::ConstantPointerNull::get(llvm::Type::getInt32Ty(TheContext)->getPointerTo());
					Builder.CreateRet(Builder.CreateBitCast(ret_val, ret_type));
        }
			}
		}
    
    blocks.pop_back();
    scopes.closeRuntimeScope();
    if(!isMain)
      Builder.SetInsertPoint(blocks.back()->getCurrentBasicBlock());
    llvm::verifyFunction(*Fun, &llvm::errs());

    return Fun;
  } 
private:
  Header *header;
  StmtBody *body;
  std::vector<AST *> local_definitions;
  TonyType *functionType;
  bool isMain;
};

#endif