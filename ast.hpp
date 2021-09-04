#ifndef __AST_HPP___
#define __AST_HPP___

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "symbol.hpp"
#include "type.hpp"
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
  virtual void printOn(std::ostream &out) const = 0;
  virtual void sem() {}; // NOTE: After we implement `sem()` for all the subclasses
                         // it would make sense to make this: `virtual void sem() = 0`.
  void llvm_compile_and_dump(){
    // Initialize
    
    TheModule = std::make_unique<llvm::Module>("tony program", TheContext);

    /* TheFPM = make_unique<legacy::FunctionPassManager>(TheModule.get());
    if (optimize) {
      TheFPM->add(createPromoteMemoryToRegisterPass());
      TheFPM->add(createInstructionCombiningPass());
      TheFPM->add(createReassociatePass());
      TheFPM->add(createGVNPass());
      TheFPM->add(createCFGSimplificationPass());
    }
    TheFPM->doInitialization(); */
    i1 = llvm::IntegerType::get(TheContext, 1);
    i8 = llvm::IntegerType::get(TheContext, 8);
    i32 = llvm::IntegerType::get(TheContext, 32);
    i64 = llvm::IntegerType::get(TheContext, 64);

    //Emit Program Code
    compile();
    bool bad = llvm::verifyModule(*TheModule, &llvm::errs());
    if (bad){
      std::cerr << "The IR is bad!\n";
      TheModule->print(llvm::errs(), nullptr);
      std::exit(1);
    }
    TheModule->print(llvm::outs(), nullptr);
  } 
  
  virtual llvm::Value *compile() = 0;
protected:
  
  static llvm::LLVMContext TheContext;
  static llvm::IRBuilder<> Builder;
  static llvm::GlobalVariable *TheVars;
  static std::unique_ptr<llvm::Module> TheModule;
  static std::map<std::string, llvm::AllocaInst*> NamedValues;

  static llvm::Type *i1;
  static llvm::Type *i8;
  static llvm::Type *i32;
  static llvm::Type *i64;
  

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
  static llvm::ConstantInt* c64(int n) {
    return llvm::ConstantInt::get(TheContext, llvm::APInt(64, n, true));
  }

  static llvm::Type* convertType(TonyType *t) {
    switch(t->get_current_type()){
      case TYPE_int: return i32;
      case TYPE_bool: return i1;
      case TYPE_char: return i8;
      default: yyerror("Type conversion not implemented yet");
      return nullptr;
    }
  }

  static llvm::AllocaInst *CreateEntryBlockAlloca (llvm::Function *TheFunction, const std::string &VarName, llvm::Type* Ty){
    llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(Ty, 0, VarName.c_str());
  }
};

inline std::ostream& operator<< (std::ostream &out, const AST &t) {
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
protected:
  TonyType* type;
};

class Stmt: public AST {
};


class Atom: public Expr { 
public:
  virtual bool isLvalue() {return false;}
  virtual std::string getName() = 0;
};

class Simple: public Stmt {  
};

class Id: public Atom {
public:
  Id(std::string v): var(v) {}
  ~Id() {}
  virtual void printOn(std::ostream &out) const override {
    out << "<Id name=\"" << var << "\"> ";
  }

  void set_type(TonyType* t){
    type = t;
  }

  void insertIntoScope(){
    st.insert(var, type);
  }

  void insertIntoParentScope(){
    st.insertIntoParentScope(var, type);
  }

  virtual std::string getName() override{
    return var;
  }

  virtual void sem() override {
    SymbolEntry *e = st.lookup(var);
    if(e == nullptr) {
      yyerror("Variable \"%s\" not found!", var.c_str());
    } 
    type = e->type;
    // For testing:
    // std::cout << "I think i saw a variable: " << var << " with type " << type <<"!\n";
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
    llvm::Value *V = NamedValues[var];
    if(!V)
      yyerror("Variable not Found");
    return Builder.CreateLoad(V, var.c_str());
  } 

  virtual bool isLvalue() override{
    return true;
  }
private:
  std::string var;
};

class ArrayElement: public Atom {
public:
  ArrayElement(Atom *a, Expr *e): atom(a), expr(e) {}
  ~ArrayElement() {delete atom; delete expr;}
  virtual void printOn(std::ostream &out) const override {
    out << "\n<ArrayElement>\n" << *atom << "\n" << *expr << "\n</ArrayElement>\n";
  }
  virtual void sem() override {
    atom->sem();
    if(atom->get_type()->get_current_type()!=TYPE_array){
      yyerror("Accessing array value of non-array object.");
    }
    expr->sem();
    if(expr->get_type()->get_current_type() != TYPE_int){
      yyerror("Expected expression to be of type integer, as an index to an array");
  }

  type = atom->get_type()->get_nested_type();
  }

  virtual bool isLvalue() override {
    return true;
  }

  virtual std::string getName() override {
    return atom->getName();
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
    return nullptr;
  } 
private:
  Atom *atom;
  Expr *expr;
};

class StringLiteral: public Atom {
public:
  StringLiteral(std::string str): strlit(str) {}
  ~StringLiteral () {}
  virtual void printOn(std::ostream &out) const override {
    out << "<String value=\"" << strlit << "\"> ";
  }

  virtual void sem() override {
    type = new TonyType(TYPE_array, new TonyType(TYPE_char, nullptr));
  }

  virtual bool isLvalue() override {
    return false;
  }

  virtual std::string getName() override {
    return strlit;
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
    return nullptr;
  } 
private:
  std::string strlit;
};

class CharConst: public Expr {
public:
  CharConst(unsigned char c): char_const(c) {}
  ~CharConst() {}
  virtual void printOn(std::ostream &out) const override {
    out << "<CharConst value='"<< char_const << "\' ascii="<< (int) char_const << "> ";
  }
  virtual void sem() override {
    type = new TonyType(TYPE_char, nullptr);
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
    return c8(char_const);
  } 
private:
  unsigned char char_const;
};

class IntConst: public Expr {
public:
  IntConst(int n): num(n) {}
  ~IntConst() {}
  virtual void printOn(std::ostream &out) const override {
    out << "<IntConst value=" << num << "> ";
  }
  virtual void sem() override {
    type = new TonyType(TYPE_int, nullptr);
  }

  virtual llvm::Value *compile () override {
    return c32(num);
  } 
private:
  int num;
};

class New: public Expr {
public:
  New(TonyType *t, Expr *right): type_of_elems(t), expr(right) {}
  ~New() {delete type_of_elems; delete expr;}
  virtual void printOn(std::ostream &out) const override {
    out << "<New> " << type_of_elems << *expr << "</New> ";
  }

  virtual void sem() override {
    expr->sem();
    if(expr->get_type()->get_current_type() != TYPE_int){
      yyerror("Array index not an integer.");
    }
    type = new TonyType(TYPE_array, type_of_elems);
    
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
    return nullptr;
  } 
private:
  TonyType *type_of_elems;
  Expr *expr;
};

/*
 *  This is a constant value (check Tony language description).
    It is equal to an empty list and has type: list[t], where `t` can be any type.
 */
class Nil: public Expr {
public:
  Nil() {}
  ~Nil() {}
  virtual void printOn(std::ostream &out) const override {
    out << "<Nil> ";
  }
  virtual void sem() override {
    type = new TonyType(TYPE_list, new TonyType(TYPE_any, nullptr));
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
    return nullptr;
  } 
};

class Boolean: public Expr {
public:
  Boolean(std::string b1): boolean_value(b1) {
    if (b1=="true") b=true;
    else b=false;
  }
  ~Boolean() {}
  virtual void printOn(std::ostream &out) const override {
    out << "<Boolean value=" << boolean_value << "> ";
  }
  virtual void sem() override {
    type = new TonyType(TYPE_bool, nullptr);
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
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
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Binop op=\"" << op << "\">\n" << *left << *right << "\n</BinOp>\n";
  }
  virtual void sem() override {
    if (op == "+" || op == "-" || op == "*" || op == "/" || op == "mod") {
      if (!left->type_check(TYPE_int) || !right->type_check(TYPE_int)) {
        // TODO: We must be more specific in our errors. This is temporary.
        yyerror("TonyType mismatch. Both expressions must be of type 'int'.\n");
      }
      type = new TonyType(TYPE_int, nullptr);
    } else if (op == "=" || op == "<>" || op == "<" || op == ">" || op == "<=" || op == ">=") {
      left->sem();
      right->sem();
      if (!check_type_equality(left->get_type(), right->get_type())) {
        yyerror("TonyType mismatch. Expressions must have the same type.\n");
      }
      type = new TonyType(TYPE_bool, nullptr);
    } else if (op == "and" || op == "or") {
      if (!left->type_check(TYPE_bool) || !right->type_check(TYPE_bool)) {
        yyerror("TonyType mismatch. Both expressions must be of type 'bool'.\n");
      }
      type = new TonyType(TYPE_bool, nullptr);
    } else if (op == "#") {
      left->sem();
      right->sem();

      if (right->get_type()->get_current_type() != TYPE_list) {
        yyerror("TonyType mismatch. Expression on the right of '#' operator \
                must be a list.\n");
      }

      if (right->get_type()->get_nested_type() != nullptr && 
          !check_type_equality(left->get_type(), right->get_type()->get_nested_type())) {
        yyerror("TonyType mismatch. Expression on the left of '#' operator \
                must be have the same type as the elements of the list on the right \
                of the operator.\n");
      }
      type = new TonyType(TYPE_list, left->get_type());
    } else {
      yyerror("Wrong binary operator.\n");
    }
  }

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
    else if(op ==  "+")     return Builder.CreateICmpSLT(l,r, "slttmp");
    else if(op ==  ">")     return Builder.CreateICmpSGT(l,r, "sgttmp");
    else if(op ==  "<=")    return Builder.CreateICmpSLE(l,r, "sletmp");
    else if(op ==  ">=")    return Builder.CreateICmpSGE(l,r, "sgetmp");
    else if(op ==  "and")   return Builder.CreateAnd(l,r, "andtmp");
    else if(op ==  "or")    return Builder.CreateOr(l,r, "ortmp");
    else if(op ==  "#")     yyerror("Operation not supported yet");
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
  virtual void printOn(std::ostream &out) const override {
    out << "\n<UnOp op=\"" << op << "\">\n" << *right << "\n</UnOp>\n";
  }
  virtual void sem() override {
    if (op == "+" || op == "-") {
      if (!right->type_check(TYPE_int)) {
        yyerror("TonyType mismatch. Expression must be of type 'int'.");
      }
      type = new TonyType(TYPE_int, nullptr);
    } else if (op == "not") { 
      if (!right->type_check(TYPE_bool)) {
        yyerror("TonyType mismatch. Expression must be of type 'bool'.");
      }
      type = new TonyType(TYPE_bool, nullptr);
    } else if (op == "head") {
      // Compute the type of the expression.
      right->sem();
      TonyType *operand_type = right->get_type();
      // Check that the expression is a list.
      if (operand_type->get_current_type() != TYPE_list) {
        yyerror("TonyType mismatch. Expression after 'head' must be a list.");
      }
      // Check that the expression is not the 'nil' constant (empty list).
      if (is_nil_constant(operand_type)) {
        yyerror("TonyType mismatch. Expression after 'head' cannot be a 'nil' list.");
      }
      // The nested type of the expression is actually the type of the list's elements.
      // NOTE: Maybe we should create a new type here, that is a copy of: 
      // `operand->get_nested_type()`. This is because, if we add a destructor in class
      // `Expr`, then the value of `type` may be deleted by multiple nodes.
      type = operand_type->get_nested_type(); 
    } else if (op == "tail") {
      // Compute the type of the expression.
      right->sem();
      TonyType *operand_type = right->get_type();
      // Check that the expression is a list.
      if (operand_type->get_current_type() != TYPE_list) {
        yyerror("TonyType mismatch. Expression after 'tail' must be a list.");
      }
      // Check that the expression is not the 'nil' constant (empty list).
      if (is_nil_constant(operand_type)) {
        yyerror("TonyType mismatch. Expression after 'tail' cannot be a 'nil' list.");
      }
      // The type of the expression is the type of the tail.
      type = operand_type; 
    } else if (op == "nil?") {
      // Compute the type of the expression.
      right->sem();
      TonyType *operand_type = right->get_type();
      if(operand_type->get_current_type() != TYPE_list) {
        yyerror("TonyType mismatch. Expression after 'nil?' must be a list.");
      }
      type = new TonyType(TYPE_bool, nullptr); 
    }
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
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

  virtual void printOn(std::ostream &out) const override {
    out << "\n<VarList>\n";
    out << type;
    for (Id * i : ids) {
      out << *i;
    }
    out << "\n</VarList>\n";
  }
  
  virtual void sem() override {
    for (Id * i : ids) {i->set_type(type); i->insertIntoScope();}
  }

  // This is called when defining variables
  virtual llvm::Value *compile() override {
    for (Id * id: ids){
      llvm::AllocaInst * Alloca = Builder.CreateAlloca(convertType(type), 0, id->getName());
      NamedValues[id->getName()] = Alloca;
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

protected:
  std::vector<Id *> ids;
  TonyType* type;
};

class Formal: public AST {
public:
  Formal(VarList* v, bool i): var_list(v), is_ref(i) {}
  ~Formal() {delete var_list;}
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Formal isRef=\"" << (is_ref ? "yes" : "no") << "\">\n" << *var_list << "</Formal>";
  }
  
  virtual void sem() override {
    var_list->sem();
  }

  //TODO: Handle refs
  std::pair<TonyType*, int> getArgs() {
    return var_list->getArgs();
  }

  std::vector<std::string> getNames() {
    return var_list->getNames();
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
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

  virtual void printOn(std::ostream &out) const override {
    out << "\n<FormalList>\n";

    for (Formal * f : formals) {
      out << *f;
    }
    out << "\n</FormalList>\n";
  }

  virtual void sem() override {
    for (Formal *f: formals) f->sem();
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
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
  virtual void sem() override {}
  virtual void printOn(std::ostream &out) const override {
    out << "<Header>\n"; 
    if(!isTyped) {
      out << "<NoType>";
    } else {
      out << type;
    }
    out << *id; 
    if(!formals) {
      out << "<NoFormalList>";
    } else {
      out << *formals;
    }
    out << "\n</Header>\n";
  }

  TonyType *getType() {return type;}

  // To handle declarations and definitions
  // The way it is structured, the function adds its own header to above function's scope
  // Declarations have headers too, but need to insert on same scope, hence 2 different sem() functions
  void semHeaderDecl() {

    // Get arguments if any
    
    //This is not needed, since a decl doesn't put any variables in scope
    //if (formals) formals->sem();

    // Get arguments if any
    std::vector<TonyType *> args;
    if (formals){
      args = formals->getArgs();
    }
    TonyType *fun;
    if (!isTyped){
      fun = new TonyType(TYPE_function, nullptr, new TonyType(TYPE_void, nullptr), args, true);
    }else{
      fun = new TonyType(TYPE_function, nullptr, type, args, true); 
    }
    id->set_type(fun);
    id->insertIntoScope();  
  }

  // TODO: Mulptiple DEFINITIONS (not declarations)
  void semHeaderDef() {
    // Get arguments if any
    if (formals) formals->sem();

    std::vector<TonyType *> args;
    if (formals){
      args = formals->getArgs();
    }
    TonyType *fun;
    if (!isTyped){
      fun = new TonyType(TYPE_function, nullptr, new TonyType(TYPE_void, nullptr), args, true);
    }else{
      fun = new TonyType(TYPE_function, nullptr, type, args, true); 
    }
    id->set_type(fun);
    
    // Check if function is previously defined
    SymbolEntry *e = st.lookupParentScope(id->getName());
    if(e != nullptr) {
      //Function either declared or defined
      TonyType *t = e->type;
      if(t->get_current_type()!= TYPE_function){
        yyerror("Expected type function.");
      }

      if(!t->isDeclared()){
        //this means function is redefined
      }

      //TODO: TonyType check if the vars in declaration match the definition
      t->toggleDeclDef();
      if(!check_type_equality(t, fun)){
        yyerror("Function definition different from declaration");
      }
      return;
    }


    if(st.hasParentScope()){
      id->insertIntoParentScope();
    }
    
  }

  bool getIsTyped(){
    return isTyped;
  }

  // Not implemented yet
  virtual llvm::Function *compile() override {
    
    llvm::Twine name = llvm::Twine(id->getName());
    std::vector<TonyType *> args;
    std::vector<std::string> names;
    if (formals){
      args = formals->getArgs();
      names = formals->getNames();
    }

    std::vector<llvm::Type *> ArgumentTypes;
    for (auto i: args){
      ArgumentTypes.push_back(convertType(i));
    }
    llvm::FunctionType *FT = llvm::FunctionType::get(i32, ArgumentTypes, false);
    llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, 
      name, TheModule.get());
    
    unsigned i = 0;
    for (auto &Arg: F->args()) Arg.setName(names[i++]);


    return F;
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
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Return>\n" << *ret_expr << "\n</Return>\n";
  }

  virtual void sem() override {
    ret_expr->sem();
    if(!check_type_equality(ret_expr->get_type(), st.getCurrentScopeReturnType())){
        yyerror("Return type different than the one declared.");
      }
    st.setScopeHasReturn();
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
    return nullptr;
  } 
private:
  Expr* ret_expr;
};

class Exit: public Stmt{
public:
  Exit() {}
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Exit>\n";
  }

  virtual void sem() override{
    TonyType *t = st.getCurrentScopeReturnType();
    if(t->get_current_type() != TYPE_void){
        yyerror("Exit from a typed function.");
    }
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
    return nullptr;
  } 
};

class StmtBody: public AST {
public:
  StmtBody(): stmts() {}
  ~StmtBody() { for (Stmt *s : stmts) delete s; }
  void append(Stmt* stmt) {
    stmts.push_back(stmt);
  }

  void reverse(){
    std::reverse(stmts.begin(), stmts.end());
  }

  virtual void printOn(std::ostream &out) const override {
    out << "\n<StmtBody>\n";
    for (Stmt *s : stmts) {
      out << *s;
    }
    out << "\n</StmtBody>\n";
  }
  virtual void sem() override {
    for (Stmt *s : stmts) {
      s->sem();
    }
    
  }

  virtual llvm::Value *compile() override {
    for (Stmt *s : stmts) {
      s->compile();
    }
    return nullptr;
  }
  
private:
  std::vector<Stmt*> stmts;
};

class Assign: public Simple {
public:
  Assign(Atom *a, Expr *e): atom(a), expr(e) {}
  ~Assign() {delete atom; delete expr;}
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Assign>\n" << *atom << *expr << "\n</Assign>\n";
  }
  virtual void sem() override {
    atom->sem();
    if (!expr->type_check(atom->get_type())) {
      yyerror("Atom on the left and expression on the right should have the same type during assignment.");
    }
    if(!atom->isLvalue()){
      yyerror("Atom is not a valid l-value.");
    }
  }
  // Not implemented yet
  virtual llvm::Value *compile() override {
    llvm::Value * Val = expr->compile();
    if(!atom->isLvalue()){
      yyerror("Atom is not a valid l-value.");
    }

    llvm::Value *Variable = NamedValues[atom->getName()];
    Builder.CreateStore(Val, Variable);
    return Val;
  } 
private:
  Atom *atom;
  Expr *expr;
};

class Skip: public Simple {
public:
  Skip() {}
  ~Skip() {}
  
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Skip>\n";
  }
  // Not implemented yet
  virtual llvm::Value *compile() override {
    return nullptr;
  } 

  void sem() override {}
};

class Elsif: public Stmt {
public:
  Elsif() {}
  ~Elsif() {
    for (Expr* e: elsif_conds) delete e;
    for (StmtBody* s: elsif_stmt_bodies) delete s;
  }
  std::vector<Expr *> get_conds() {
    return elsif_conds;
  }
  std::vector<StmtBody *> get_stmt_bodies() {
    return elsif_stmt_bodies;
  }

  void clear(){
    elsif_conds.clear();
    elsif_stmt_bodies.clear();
  }

  bool isEmpty(){
    return elsif_conds.empty();
  }
  void append(Expr* e, StmtBody* s) {
    elsif_conds.push_back(e);
    elsif_stmt_bodies.push_back(s);
  }

    virtual void printOn(std::ostream &out) const override {
    // This is never used
    out << "<Elsif> </Elsif>";
  }
  virtual void sem() override {}
  // Not implemented yet
  virtual llvm::Value *compile() override {
    return nullptr;
  } 

private:
  std::vector<Expr *> elsif_conds;
  std::vector<StmtBody *> elsif_stmt_bodies;
};

class Else: public Stmt {
public:
  Else() {}
  Else(StmtBody *s){else_stmt.push_back(s);}
  ~Else() {
    if (else_stmt.empty()){
      delete else_stmt[0];
    }
  }
  StmtBody * get_stmt() {
    return else_stmt[0]; 
  }

  void clear(){
    else_stmt.clear();
  }

  bool isEmpty(){
    return else_stmt.empty();
  }

  // This is never used, only for testing
  virtual void printOn(std::ostream &out) const override {
    if(!else_stmt.empty()){
      out << "\n<Else>\n" << *else_stmt[0] << "\n</Else>\n";
    }
  }

  virtual void sem() override {}
  // Not implemented yet
  virtual llvm::Value *compile() override {
    return nullptr;
  } 

private:
  std::vector<StmtBody *> else_stmt;
};

class If: public Stmt {
public:
  If(Expr *if_condition, StmtBody *if_stmt_body, Elsif *elsif_stmt, Else *else_stmt){
    conditions.push_back(if_condition);
    statements.push_back(if_stmt_body);
    
    // TODO: There might exist a more efficient way to do this.
    std::vector<Expr *> elsif_conds = elsif_stmt->get_conds();
    std::vector<StmtBody *> elsif_stmt_bodies = elsif_stmt->get_stmt_bodies();
    for(int i=0; i < (int) elsif_conds.size(); i++){
      conditions.push_back(elsif_conds[i]);
      statements.push_back(elsif_stmt_bodies[i]);
    }

    if(!else_stmt->isEmpty()) {
      conditions.push_back(new Boolean("true"));
      statements.push_back(else_stmt->get_stmt());
      hasElse=true;
    }
  }
  ~If() {
    for (Expr* e: conditions) delete e;
    for (StmtBody* s: statements) delete s; 
  }

  virtual void printOn(std::ostream &out) const override {
    out << "\n<If>\n";
    out << *conditions[0] << *statements[0];

    int size = (int) conditions.size();

    if(hasElse) {
      for (int i=1; i<(size-1); i++) {
        out << "\n<ElsIf>\n" << *conditions[i] << *statements[i] << "\n</ElsIf>\n";
      }
      out << "\n<Else>\n" << *conditions[size-1] << *statements[size-1] << "\n</Else>\n";
    } else {
      for (int i=1; i<size; i++) {
        out << "\n<ElsIf>\n" << *conditions[i] << *statements[i] << "\n</ElsIf>\n";
      }
    }
    out << "\n</If>\n";
  }

  virtual void sem() override {
    // TonyType-check all conditions and call `sem()` for all statements.
    for(int i=0; i<(int) conditions.size(); i++) {
      if(!conditions[i]->type_check(TYPE_bool)) {
        yyerror("TonyType mismatch. 'If-condition' is not boolean.");
      }
      statements[i]->sem();
    }
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
    return nullptr;
  } 

private:
  std::vector<Expr *>     conditions;
  std::vector<StmtBody *> statements;
  bool hasElse = false;
};

class SimpleList: public AST {
public:
  SimpleList(): simples() {}
  ~SimpleList() { for (Simple *s : simples) delete s; }

  void append(Simple *s) { simples.push_back(s); }

  void reverse(){
    std::reverse(simples.begin(), simples.end());
  }

  virtual void printOn(std::ostream &out) const override {
    out << "\n<SimplesList>\n";
    for (Simple *s : simples) {
      out << *s;
    }
    out << "\n</SimpleList>\n";
  }
  std::vector<Simple *> get_simples_list(){
    return simples;
  }

  virtual void sem() override {
    for (Simple *s : simples) {
      s->sem();
    }
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
    return nullptr;
  } 
private:
  std::vector<Simple *> simples;
};

class For: public Stmt {
public:
  For(SimpleList *sl1, Expr *e, SimpleList *sl2, StmtBody *sb):
    simple_list_1(sl1), expr(e), simple_list_2(sl2), stmt_body(sb) {}
  ~For() {
    delete simple_list_1;
    delete expr;
    delete simple_list_2;
    delete stmt_body;
  }

  virtual void printOn(std::ostream &out) const override {
    out << "\n<For>\n" << *simple_list_1 << *expr << *simple_list_2  << *stmt_body << "\n</For>\n";
  }

  virtual void sem() override {
    simple_list_1->sem();
    expr->sem();
    if(expr->get_type()->get_current_type() != TYPE_bool){
      yyerror("For condition is not boolean.");
    }
    simple_list_2->sem();
    stmt_body->sem();
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
    return nullptr;
  } 

private:
  SimpleList *simple_list_1;
  Expr *expr;
  SimpleList *simple_list_2;
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

  virtual void printOn(std::ostream &out) const override {
    out << "\n<ExprList>\n";
    for (Expr *e : expressions) {
      out << *e;
    }
    out << "\n</ExprList>\n";
  }

  std::vector<Expr*> get_expr_list(){
    return expressions;
  }

  virtual void sem() override {
    for (auto i : expressions) i->sem();
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
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
  virtual void printOn(std::ostream &out) const override {
    if(!hasParams)
      out << "\n<FunctionCall>\n" << *name << "\n</FunctionCall>\n";
    else
      out << "\n<FunctionCall>\n" << *name << *params << "\n</FunctionCall>\n";
  }

  virtual void sem() override {
    name->sem();

    if (name->get_type()->get_current_type() != TYPE_function)
      yyerror("Function call, expected a function");
    std::vector<TonyType *> args = name->get_type()->get_function_args();
    std::vector<Expr*> expressions;
    if (hasParams){
      expressions = params->get_expr_list();
    }

    if(expressions.size() != args.size()){
      yyerror("Function call: Different number of arguments than expected");
    }

    for (int i=0; i<(int) args.size();++i){
      expressions[i]->sem();
      if(!check_type_equality(args[i],expressions[i]->get_type())){
        yyerror("Function call: Different argument type than expected");
      }
    }
    type = name->get_type()->get_return_type();
  }

  virtual bool isLvalue() override {
    return false;
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
    return nullptr;
  } 

  virtual std::string getName() override {
    return name->getName();
  }

private:
  Id *name;
  ExprList *params;
  bool hasParams;
};

class FunctionDeclaration: public AST {
public:
  FunctionDeclaration(Header *hd): header(hd){}
  ~FunctionDeclaration() {delete header;}
  virtual void printOn(std::ostream &out) const override {
    out << "\n<FunctionDeclaration>\n" << *header << "\n</FunctionDeclaration>\n" ;
  }

  virtual void sem() override {
    header->semHeaderDecl();
  }

  // Not implemented yet
  virtual llvm::Value *compile() override {
    return nullptr;
  } 

private:
  Header *header;
};

class FunctionDefinition: public AST {
public:
  FunctionDefinition(): header(), body(){}
  ~FunctionDefinition() {
    delete header; delete body;
    for (AST *a : local_definitions) delete a;
  }

  void initFunctions(){
    //puti
    std::vector<TonyType*> v {new TonyType(TYPE_int, nullptr)};
    st.insert(std::string("puti"), new TonyType(TYPE_function, nullptr,new TonyType(TYPE_void, nullptr), v, true));
    //putc
    v.clear();
    v.push_back(new TonyType(TYPE_char, nullptr));
    st.insert(std::string("putc"), new TonyType(TYPE_function, nullptr,new TonyType(TYPE_void, nullptr), v, true));

    //putb
    v.clear();
    v.push_back(new TonyType(TYPE_bool, nullptr));
    st.insert(std::string("putb"), new TonyType(TYPE_function, nullptr,new TonyType(TYPE_void, nullptr), v, true));
    
    //puts
    v.clear();
    v.push_back(new TonyType(TYPE_array, new TonyType(TYPE_char, nullptr)));
    st.insert(std::string("puts"), new TonyType(TYPE_function, nullptr,new TonyType(TYPE_void, nullptr), v, true));

    //geti
    v.clear();
    st.insert(std::string("geti"), new TonyType(TYPE_function, nullptr, new TonyType(TYPE_int, nullptr), v, true));

    //getb
    v.clear();
    st.insert(std::string("getb"), new TonyType(TYPE_function, nullptr, new TonyType(TYPE_bool, nullptr), v, true));

    //getc
    v.clear();
    st.insert(std::string("getc"), new TonyType(TYPE_function, nullptr, new TonyType(TYPE_char, nullptr), v, true));

    //gets
    v.clear();
    v.push_back(new TonyType(TYPE_int, nullptr));
    v.push_back(new TonyType(TYPE_array, new TonyType(TYPE_char, nullptr)));
    st.insert(std::string("gets"), new TonyType(TYPE_function, nullptr, new TonyType(TYPE_void, nullptr), v, true));

    //abs
    v.clear();
    v.push_back(new TonyType(TYPE_int, nullptr));
    st.insert(std::string("abs"), new TonyType(TYPE_function, nullptr, new TonyType(TYPE_int, nullptr), v, true));

    //ord
    v.clear();
    v.push_back(new TonyType(TYPE_char, nullptr));
    st.insert(std::string("ord"), new TonyType(TYPE_function, nullptr, new TonyType(TYPE_int, nullptr), v, true));

    //chr
    v.clear();
    v.push_back(new TonyType(TYPE_int, nullptr));
    st.insert(std::string("chr"), new TonyType(TYPE_function, nullptr, new TonyType(TYPE_char, nullptr), v, true));

    //strlen
    v.clear();
    v.push_back(new TonyType(TYPE_array, new TonyType(TYPE_char, nullptr)));
    st.insert(std::string("strlen"), new TonyType(TYPE_function, nullptr, new TonyType(TYPE_int, nullptr), v, true));

    //strcmp
    v.clear();
    v.push_back(new TonyType(TYPE_array, new TonyType(TYPE_char, nullptr)));
    v.push_back(new TonyType(TYPE_array, new TonyType(TYPE_char, nullptr)));
    st.insert(std::string("strcmp"), new TonyType(TYPE_function, nullptr, new TonyType(TYPE_int, nullptr), v, true));

    //strcpy
    v.clear();
    v.push_back(new TonyType(TYPE_array, new TonyType(TYPE_char, nullptr)));
    v.push_back(new TonyType(TYPE_array, new TonyType(TYPE_char, nullptr)));
    st.insert(std::string("strcpy"), new TonyType(TYPE_function, nullptr, new TonyType(TYPE_void, nullptr), v, true));

    //strcat
    v.clear();
    v.push_back(new TonyType(TYPE_array, new TonyType(TYPE_char, nullptr)));
    v.push_back(new TonyType(TYPE_array, new TonyType(TYPE_char, nullptr)));
    st.insert(std::string("strcat"), new TonyType(TYPE_function, nullptr, new TonyType(TYPE_void, nullptr), v, true));

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

  virtual void printOn(std::ostream &out) const override {
    out << "\n<FunctionDefinition>\n" << *header; 
    
    for (AST *a : local_definitions) out << *a;

    out << *body << "\n</FunctionDefinition>\n" ;
  }

  virtual void sem() override {
    st.openScope(header->getType());
  
    if(!st.hasParentScope()){
      initFunctions();
    }
    header->semHeaderDef();
    for (AST *a : local_definitions) a->sem();
    body->sem();
    if(header->getIsTyped() && !st.getScopeHasReturn()){
      yyerror("No return value on typed function.");
    }
    //st.printSymbolTable();
    st.closeScope();
  }

  virtual llvm::Value *compile () override {
    
    // Create the function based on header
    llvm::Function *fun = header->compile();
    
    //Create Basic block
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", fun);
    Builder.SetInsertPoint(BB);    
    
    //Insert values into table
    NamedValues.clear();
    for (auto &arg: fun->args()) {
      llvm::AllocaInst * Alloca = CreateEntryBlockAlloca(fun, arg.getName().str(), arg.getType());

      Builder.CreateStore(&arg, Alloca);
      NamedValues[arg.getName().str()] = Alloca;
    }
    for(AST *a: local_definitions) a->compile();
    body->compile();
    Builder.CreateRet(c32(0));
    
    return nullptr;
  } 




private:
  Header *header;
  StmtBody *body;
  std::vector<AST *> local_definitions;
};

#endif