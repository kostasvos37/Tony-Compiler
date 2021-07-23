#ifndef __AST_HPP___
#define __AST_HPP___

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>


class AST{
public:
  virtual ~AST() {}
  virtual void printOn(std::ostream &out) const = 0;
};

inline std::ostream& operator<< (std::ostream &out, const AST &t) {
  t.printOn(out);
  return out;
}


// Virtual to avoid "diamond of death"
class Expr: virtual public AST {
};


class Stmt: virtual public AST {
};


class Atom: public Expr { 
};


class Simple: public Stmt {  
};


class Type: public AST{
public:
  Type(std::string str): type(str), hasSubtype(false){}
  Type(std::string str, Type * typ): type(str), hasSubtype(true), subtype(typ){}
  ~Type(){
    if(hasSubtype) delete subtype;
  }

  virtual void printOn(std::ostream &out) const override {
    out << " <Type> " << type;
    if (hasSubtype){
      out << *subtype;
    }
    out << " </Type> ";

  }
private:
  std::string type;
  bool hasSubtype;
  Type * subtype;
};


class Id: public Atom {
public:
  Id(std::string v): var(v) {}
  virtual void printOn(std::ostream &out) const override {
    out << "<Id name=\"" << var << "\"> ";
  }
private:
  std::string var;
};


class Array: public Atom {
public:
  Array(Atom *a, Expr *e): atom(a), expr(e) {}
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Array>\n" << *atom << "\n" << *expr << "\n</Array>\n";
  }
private:
  Atom *atom;
  Expr *expr;
};


class StringLiteral: public Atom {
public:
  StringLiteral(std::string str): strlit(str) {}
  virtual void printOn(std::ostream &out) const override {
    out << "<String value=\"" << strlit << "\"> ";
  }
private:
  std::string strlit;
};


class CharConst: public Expr {
public:
  CharConst(char c): char_const(c) {}
  virtual void printOn(std::ostream &out) const override {
    out << "<CharConst value='" << char_const << "'> ";
  }
private:
  char char_const;
};


class IntConst: public Expr {
public:
  IntConst(int n): num(n) {}
  virtual void printOn(std::ostream &out) const override {
    out << "<IntConst value=" << num << "> ";
  }
private:
  int num;
};


class New: public Expr {
public:
  New(Type *t, Expr *right): typ(t), expr(right){}
  ~New() {delete expr;}
  virtual void printOn(std::ostream &out) const override {
    out << "<New> " << *typ << *expr << "</New> ";
  }
private:
  Type *typ;
  Expr *expr;
};


class Nil: public Expr {
public:
  Nil() {}
  virtual void printOn(std::ostream &out) const override {
    out << "<Nil> ";
  }
};


class Boolean: public Expr {
public:
  Boolean(std::string b): boo(b) {} //wink wink pap
  virtual void printOn(std::ostream &out) const override {
    out << "<Boolean value=" << boo << "> ";
  }
private:
  std::string boo;
};


class BinOp: public Expr {
public:
  BinOp(Expr *l, std::string o, Expr *r): left(l), op(o), right(r) {}
  ~BinOp() { delete left; delete right; }
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Binop op=\"" << op << "\">\n" << *left << *right << "\n</BinOp>\n";
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
private:
  std::string op;
  Expr *right;
};


class VarList: public AST {
public:
  VarList() {}
  ~VarList(){
    for (Id * i : ids) delete i;
  }
  void append(Id * id) {
    ids.push_back(id);
  }
  void set_type(Type *t){
    type = t;
  }

  virtual void printOn(std::ostream &out) const override {
    out << "\n<VarList>\n";
    out << *type;
    for (Id * i : ids) {
      out << *i;
    }
    out << "\n</VarList>\n";
  }
private:
  std::vector<Id *> ids;
  Type * type;
};


class Formal: public AST {
public:
  Formal(VarList* v, bool isref): varlist(v), isRef(isref) {}
virtual void printOn(std::ostream &out) const override {
    out << "\n<Formal isRef=\"" << (isRef ? "yes" : "no") << "\">\n" << *varlist << "</Formal>";
  }
private:
  VarList* varlist;
  bool isRef;
};


class FormalList: public AST {
public:
  FormalList() {}
  ~FormalList(){
    for (Formal * f : formals) delete f;
  }

  void append(Formal * f) {
    formals.push_back(f);
  }

  virtual void printOn(std::ostream &out) const override {
    out << "\n<FormalList>\n";

    for (Formal * f : formals) {
      out << *f;
    }
    out << "\n</FormalList>\n";
  }
private:
  std::vector<Formal *> formals;
};


class Header: public AST {
public:
  Header(Type *t, Id *i, FormalList *f): type(t), formals(f), id(i) {}
  virtual void printOn(std::ostream &out) const override {
    out << "<Header>\n"; 
    if(type == NULL){
      out << "<NoType>";
    }else{
      out << *type;
    }
    out << *id; 
    if(formals == NULL){
      out << "<NoFormalList>";
    }else{
      out << *formals;
    }
    out << "\n</Header>\n";
  }
private:
  Type *type;
  FormalList* formals;
  Id *id;
};


class Return: public Stmt{
public:
  Return(Expr* e): ret_expr(e) {}
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Return>\n" << *ret_expr << "\n</Return>\n";
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
};


class StmtBody: public AST {
public:
  StmtBody(): stmts() {}
  ~StmtBody() { for (Stmt *s : stmts) delete s; }
  void append(Stmt* stmt) {
    stmts.push_back(stmt);
    return;
  }
  virtual void printOn(std::ostream &out) const override {
    out << "\n<StmtBody>\n";
    for (Stmt *s : stmts) {
      out << *s;
    }
    out << "\n</StmtBody>\n";
  }
  
private:
  std::vector<Stmt*> stmts;
};


class Assign: public Simple{
public:
  Assign(Atom *a, Expr *e): atom(a), expr(e) {}
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Assign>\n" << *atom << *expr << "\n</Assign>\n";
  }
private:
  Atom *atom;
  Expr *expr;
};


class Skip: public Simple{
public:
  Skip() {}
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Skip>\n";
  }
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

  bool isEmpty(){
    return elsif_conds.empty();
  }
  void append(Expr* e, StmtBody* s) {
    elsif_conds.push_back(e);
    elsif_stmt_bodies.push_back(s);
  }

  virtual void printOn(std::ostream &out) const override {
    
    // This is never used
    if(!elsif_conds.empty()){
      out << "<Elsif> ";
      for(int i=0; i< (int) elsif_stmt_bodies.size(); i++){
        out << "( " << *elsif_conds[i] << ", " << *elsif_stmt_bodies[i] << ")";
      }
      out << ")\n";
    }
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

  bool isEmpty(){
    return else_stmt.empty();
  }

  // This is never used, only for testing
  virtual void printOn(std::ostream &out) const override {
    
    if(!else_stmt.empty()){
      out << "\n<Else>\n" << *else_stmt[0] << "\n</Else>\n";
    }
  }

private:
  std::vector<StmtBody *> else_stmt;
};


class If: public Stmt {
public:
  If(Expr *e, StmtBody *b, Elsif * ei, Else *el){
    conditions.push_back(e);
    statements.push_back(b);
    
    std::vector<Expr *> elsif_conds = ei->get_conds();
    std::vector<StmtBody *> elsif_stmt_bodies = ei->get_stmt_bodies();
    for(int i=0; i< (int) elsif_conds.size();i++){
      conditions.push_back(elsif_conds[i]);
      statements.push_back(elsif_stmt_bodies[i]);
    }

    if(!el->isEmpty()){
      conditions.push_back(new Boolean("true"));
      statements.push_back(el->get_stmt());
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
    int size=(int) conditions.size();
    if(hasElse){
      for (int i=1; i< (size-1);i++) {
        out << "\n<ElsIf>\n" << *conditions[i] << *statements[i] << "\n</ElsIf>\n";
      }
      out << "\n<Else>\n" << *conditions[size-1] << *statements[size-1] << "\n</Else>\n";
    }else{
      for (int i=1; i< (size);i++) {
        out << "\n<ElsIf>\n" << *conditions[i] << *statements[i] << "\n</ElsIf>\n";
      }
    }
    out << "\n</If>\n";
  }

private:
  std::vector<Expr *> conditions;
  std::vector<StmtBody *> statements;
  bool hasElse = false;
};


class SimpleList: public AST {
public:
  SimpleList(): simples() {}
  ~SimpleList() { for (Simple *s : simples) delete s; }

  void append(Simple *s) { simples.push_back(s); }
  void insert_front(Simple *s) { simples.insert(simples.begin(), s);}

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
private:
  std::vector<Simple *> simples;
};


class For: public Stmt {
public:
  For(SimpleList *sl1, Expr *e, SimpleList *sl2, StmtBody *sb):
    simple_list1(sl1), expr(e), simple_list2(sl2), stmt_body(sb)  {}
  ~For() {
    delete simple_list1;
    delete expr;
    delete simple_list2;
    delete stmt_body; }
  virtual void printOn(std::ostream &out) const override {
    out << "\n<For>\n" << *simple_list1 << *expr << *simple_list2  << *stmt_body << "\n</For>\n";
  }

private:
  SimpleList *simple_list1;
  Expr *expr;
  SimpleList *simple_list2;
  StmtBody *stmt_body;
};


class ExprList: public AST {
public:
  ExprList(): expressions() {}
  ~ExprList() { for (Expr *e : expressions) delete e; }

  void append(Expr *e) { expressions.push_back(e); }
  void insert_front(Expr *e) { expressions.insert(expressions.begin(), e);}

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
private:
  std::vector<Expr*> expressions;
};


class FunctionCall: public Simple, public Atom {
public:
  FunctionCall(Id *n): name(n), hasParams(false) {}
  FunctionCall(Id *n, ExprList *el): name(n), params(el), hasParams(true) {}
  virtual void printOn(std::ostream &out) const override {
    if(!hasParams)
      out << "\n<FunctionCall>\n" << *name << "\n</FunctionCall>\n";
    else
      out << "\n<FunctionCall>\n" << *name << *params << "\n</FunctionCall>\n";
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

private:
  Header *header;
};


class FunctionDefinitionList: public AST {
public:
  FunctionDefinitionList(){}
  ~FunctionDefinitionList() {for (AST *a : definitions) delete a;}

  void append(AST* a) {
    definitions.push_back(a);
  }

  virtual void printOn(std::ostream &out) const override {
    out << "\n<FunctionDefinitionList>\n"; 
    
    for (AST *a : definitions) out << *a;
    out <<"\n</FunctionDefinitionList>\n" ;
  }

private:
  std::vector<AST *> definitions;
};


class FunctionDefinition: public AST {
public:
  FunctionDefinition(Header * hd, FunctionDefinitionList *fdl, StmtBody *stb):
  header(hd), definitions(fdl), body(stb){}
  ~FunctionDefinition() {delete header; delete definitions; delete body;}

  virtual void printOn(std::ostream &out) const override {
    out << "\n<FunctionDefinition>\n" << *header << *definitions << *body <<"\n</FunctionDefinition>\n";
  }

private:
  Header *header;
  FunctionDefinitionList *definitions;
  StmtBody *body;
};


#endif