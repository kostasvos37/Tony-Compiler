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
  void llvm_compile_and_dump(bool optimize);
  void optimizeModule();
  void initializeLibraryFunctions();
  
  virtual llvm::Value *compile() = 0;
  virtual void setLineno(int n);
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
    
  static llvm::ConstantInt* c1(bool b);
  static llvm::ConstantInt* c8(char c);
  static llvm::ConstantInt* c32(int n);
  static llvm::Type* getOrCreateLLVMTypeFromTonyType(TonyType *t, PassMode mode = VAL) ;
  static llvm::Type *getLLVMRefType(llvm::Type *orgType);
  static llvm::AllocaInst *CreateEntryBlockAlloca (llvm::Function *TheFunction, const std::string &VarName, llvm::Type* Ty);
};

inline std::ostream& operator<< (std::ostream &out, AST &t) {
  t.printOn(out);
  return out;
}

class Expr: public AST {
public:
  TonyType* get_type();
  void setLLVMType(llvm::Type* t);

protected:
  TonyType* type;
  llvm::Type* LLVMType;
};

class Stmt: public AST {
public:
};


class Atom: public Expr { 
public:
  virtual bool isLvalue();
  virtual std::string getName() = 0;
  virtual void setPassByValue(bool b);

protected:
  bool pass_by_value;
};

class Simple: public Stmt {  
};

class Id: public Atom {
public:
  // Parsing
  Id(std::string v);
  ~Id();
  virtual void printOn(std::ostream &out) override;

  // Semantic
  void set_type(TonyType* t);
  void insertIntoScope(TableType l);
  void insertIntoParentScope(TableType l);
  virtual std::string getName() override;
  virtual bool isLvalue() override;
  virtual void sem() override; 

  // Compile
  virtual llvm::Value *compile() override;
private:
  std::string var;
};

class ArrayElement: public Atom {
public:
  // Parsing
  ArrayElement(Atom *a, Expr *e);
  ~ArrayElement();
  virtual void printOn(std::ostream &out) override;

  // Semantic
  virtual bool isLvalue() override;
  virtual std::string getName() override;
  virtual void sem() override;

  // Compile
  virtual llvm::Value *compile() override;
private:
  Atom *atom;
  Expr *expr;
};

class StringLiteral: public Atom {
public:
  // Parsing
  StringLiteral(std::string str);
  ~StringLiteral ();
  virtual void printOn(std::ostream &out) override;

  // Semantic
  virtual void sem() override;
  virtual bool isLvalue() override;
  virtual std::string getName() override;

  // Compile
  virtual llvm::Value *compile() override;
private:
  std::string strlit;
};

class CharConst: public Expr {
public:
  // Parsing
  CharConst(unsigned char c);
  ~CharConst() ;
  virtual void printOn(std::ostream &out) override;
  
  // Semantic
  virtual void sem() override;

  // Compile
  virtual llvm::Value *compile() override;
private:
  unsigned char char_const;
};

class IntConst: public Expr {
public:
  // Parsing
  IntConst(int n);
  ~IntConst();
  virtual void printOn(std::ostream &out) override;
  
  // Semantic
  virtual void sem() override;

  // Compile
  virtual llvm::Value *compile () override;
private:
  int num;
};

class New: public Expr {
public:
// Parsing
  New(TonyType *t, Expr *right);
  ~New();
  virtual void printOn(std::ostream &out) override;
  
  // Semantic
  virtual void sem() override;

  // Compile
  virtual llvm::Value *compile() override;
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
  // Parsing
  Nil() ;
  ~Nil() ;
  virtual void printOn(std::ostream &out) override;
  
  // Semantic
  virtual void sem() override;
  
  // Compile
  virtual llvm::Value *compile() override;
};

class Boolean: public Expr {
public:
  // Parsing
  Boolean(std::string b1);
  ~Boolean();
  virtual void printOn(std::ostream &out) override;

  // Semantic
  virtual void sem() override;

  // Compile
  virtual llvm::Value *compile() override;
private:
  std::string boolean_value;
  bool b;
};

class BinOp: public Expr {
public:
  // Parsing
  BinOp(Expr *l, std::string o, Expr *r);
  ~BinOp();
  virtual void printOn(std::ostream &out) override;
  
  //Semantic
  virtual void sem() override;

  // Compile
  virtual llvm::Value *compile() override;
private:
  Expr *left;
  std::string op;
  Expr *right;
};

class UnOp: public Expr {
public:
  // Parsing
  UnOp(std::string(o), Expr *r);
  ~UnOp();
  virtual void printOn(std::ostream &out) override;

  // Semantic
  virtual void sem() override;

  // Compile
  virtual llvm::Value *compile() override;
private:
  std::string op;
  Expr *right;
};

class VarList: public AST {
public:
  // Parsing
  VarList() ;
  ~VarList() ;
  virtual void printOn(std::ostream &out) override;
  void append(Id * id);
  void reverse();

  // Semantic
  std::pair<TonyType*, int> getArgs();
  std::vector<std::string> getNames();
  void setIsRef(bool b);
  void set_type(TonyType* t);
  virtual void sem() override;

  // Compile
  virtual llvm::Value *compile() override;
protected:
  std::vector<Id *> ids;
  TonyType* type;
  bool isRef;
};

class Formal: public AST {
public:
  // Parsing
  Formal(VarList* v, bool i);
  ~Formal() ;
  virtual void printOn(std::ostream &out) override;
  
  // Semantic
  std::pair<TonyType*, int> getArgs() ;
  std::vector<std::string> getNames();
  virtual void sem() override;

  // Compile
  virtual llvm::Value *compile() override;

private:
  VarList* var_list;
  bool is_ref;
};

class FormalList: public AST {
public:
  // Parsing
  FormalList() ;
  ~FormalList() ;
  void append(Formal * f);
  void reverse();
  virtual void printOn(std::ostream &out) override;

  // Semantic
  virtual void sem() override;
  std::vector<TonyType *> getArgs();
  std::vector<std::string> getNames();

  // Compile
  virtual llvm::Value *compile() override; 
private:
  std::vector<Formal *> formals;
};

class Header: public AST {
public:
  // Parsing
  Header(TonyType *t, Id *name, FormalList *f);
  Header(Id *name, FormalList *f);
  ~Header();
  virtual void sem() override;
  virtual void printOn(std::ostream &out) override;

  // Semantic
  // To handle declarations and definitions
  // The way it is structured, the function adds its own header to above function's scope
  // Declarations have headers too, but need to insert on same scope, hence 2 different sem() functions
  void semHeaderDecl();
  void semHeaderDef();
  bool getIsTyped();
  std::string getName();
  std::vector<TonyType *> getArgs();
  std::vector<std::string> getNames();
  TonyType *getType();

  // Compile
  virtual llvm::Value *compile() override; 

private:
  TonyType *type;
  FormalList *formals;
  Id *id;
  bool isTyped;
};

class Return: public Stmt{
public:
  // Parsing
  Return(Expr* e);
  ~Return();
  virtual void printOn(std::ostream &out) override;

  // Semantic
  virtual void sem() override;

  // Compile
  virtual llvm::Value *compile() override;
private:
  Expr* ret_expr;
};

class Exit: public Stmt{
public:
  // Parsing
  Exit() ;
  ~Exit();
  virtual void printOn(std::ostream &out) override;

  // Semantic
  virtual void sem() override;

  // Compile
  virtual llvm::Value* compile() override;
};

class StmtBody: public AST {
public:
  // Parsing
  StmtBody();
  ~StmtBody() ;
  void append(Stmt* stmt);
  void reverse();
  virtual void printOn(std::ostream &out) override;
  
  // Semantic
  bool hasReturnStmt() ;
  bool hasExitStmt() ;
  virtual void sem() override;

  // Compile
  virtual llvm::Value *compile() override;
private:
  std::vector<Stmt*> stmts;
  bool has_return, has_exit;
};

class Assign: public Simple {
public:
  // Parsing
  Assign(Atom *a, Expr *e);
  ~Assign();
  virtual void printOn(std::ostream &out) override;
  
  // Semantic
  virtual void sem() override;

  // Compile
  virtual llvm::Value* compile() override;
private:
  Atom *atom;
  Expr *expr;
};

class Skip: public Simple {
public:
  // Parsing
  Skip() ;
  ~Skip();
  virtual void printOn(std::ostream &out) override;

  // Semantic
  virtual void sem() override;

  // Compile
  virtual llvm::Value *compile() override;
};


/*
 * This class represents Tony's `if`, `elsif` and `else` statements, not just `if`.
 */
class If: public Stmt {
public:
  If(Expr* if_condition, StmtBody* if_stmt_body, If* next);
  ~If() ;
  
  virtual void printOn(std::ostream &out) override;

  // Semantic
  virtual bool isElse();
  virtual StmtBody* getStmtBody();
  virtual void sem() override;

  // Compile
  virtual llvm::Value* compile() override;
private:
  Expr*     condition;
  StmtBody* stmt_body;
  If*       next_if;
};

class SimpleList: public AST {
public:
  // Parsing
  SimpleList() ;
  ~SimpleList() ;
  void append(Simple *s);
  void reverse();
  virtual void printOn(std::ostream &out) override;
  
  // Semantic
  std::vector<Simple *> get_simples_list();
  virtual void sem() override;

  // Compile
  virtual llvm::Value *compile() override;
private:
  std::vector<Simple *> simples;
};

class For: public Stmt {
public:
  // Parsing
  For(SimpleList *sl1, Expr *e, SimpleList *sl2, StmtBody *sb);
  ~For() ;
  virtual void printOn(std::ostream &out) override;

  // Semantic
  virtual void sem() override;

  // Compile
  virtual llvm::Value *compile() override;

private:
  SimpleList *initializations;
  Expr *condition;
  SimpleList *steps;
  StmtBody *stmt_body;
};

class ExprList: public AST {
public:
  // Parsing
  ExprList();
  ~ExprList() ;
  virtual void printOn(std::ostream &out) override;
  void append(Expr *e);
  void reverse();

  // Semantic
  std::vector<Expr*> get_expr_list();
  virtual void sem() override;

  // Compile
  virtual llvm::Value* compile() override;
private:
  std::vector<Expr*> expressions;
};

class FunctionCall: public Simple, public Atom {
public:
  // Parsing
  FunctionCall(Id *n);
  FunctionCall(Id *n, ExprList *el);
  ~FunctionCall();
  virtual void printOn(std::ostream &out) override;

  // Semantic
  virtual std::string getName() override;
  virtual bool isLvalue() override;
  void setLineno(int n) override;
  virtual void sem() override;

  // Compile
  virtual llvm::Value* compile() override;
private:
  Id *name;
  ExprList *params;
  bool hasParams;
  int lineno;
};

class FunctionDeclaration: public AST {
public:
  // Parsing
  FunctionDeclaration(Header *hd);
  ~FunctionDeclaration();
  virtual void printOn(std::ostream &out) override;

  // Semantic
  virtual void sem() override;

  // Compile
  virtual llvm::Value *compile() override;
private:
  Header *header;
  TonyType *functionType;
};

class FunctionDefinition: public AST {
public:
  // Parsing
  FunctionDefinition();
  ~FunctionDefinition();
  void append(AST* a) ;
  void merge(Header *hd, StmtBody *st);
  void reverse();
  virtual void printOn(std::ostream &out) override;

  // Semantic
  void initFunctions();
  std::string getName();
  void setIsMain();
  virtual void sem() override;

  // Compile
  virtual llvm::Value* compile() override; 
private:
  Header *header;
  StmtBody *body;
  std::vector<AST *> local_definitions;
  TonyType *functionType;
  bool isMain;
};

#endif