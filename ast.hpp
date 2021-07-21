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
  virtual void compile() const = 0;
};

inline std::ostream& operator<< (std::ostream &out, const AST &t) {
  t.printOn(out);
  return out;
}

class Expr: public AST {
public:
  virtual void compile() const = 0;  
};

class Stmt: public AST {
public:
  virtual void compile() const = 0;  
};

class Atom: public Expr { 
};

class RValue: public Expr {  
};

class Simple: public Stmt {  
};













class If: public Stmt {
public:
  If(Expr *e, StmtBody *b, Elsif * ei, Else *el){
    conditions[0] = e;
    statements[0] = b;
    
    std::vector<Expr *> elsif_conds = ei->get_conds();
    std::vector<StmtBody *> elsif_stmt_bodies = ei->get_stmt_bodies();
    for(int i=0; i<elsif_conds.size();i++){
      conditions.push_back(elsif_conds[i]);
      statements.push_back(elsif_stmt_bodies[i]);
    }

    if(!el->isEmpty()){
      conditions.push_back(new Boolean("true"));
      statements.push_back(el->get_stmt());
    }
  }
  ~If() {
    for (Expr* e: conditions) delete e;
    for (StmtBody* s: statements) delete s; 
  }

    virtual void printOn(std::ostream &out) const override {
    out << "If(";
    
    for (int i=0; i< conditions.size();i++) {
      out << *conditions[i];
      out << *statements[i];
    }
    out << ")\n";
  }
  virtual void compile() const override {
    printf("If statement and these are my iffies: \n");
    for (int i=0; i<conditions.size();i++){
      conditions[i]->compile();
      statements[i]->compile();
    }
  }
private:
  std::vector<Expr *> conditions;
  std::vector<StmtBody *> statements;
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
    
    if(!elsif_conds.empty()){
      out << "ElseIf ( ";
      for(int i=0; i<elsif_stmt_bodies.size(); i++){
        out << "( " << *elsif_conds[i] << ", " << *elsif_stmt_bodies[i] << ")";
      }
      out << ")\n";
    }
  }
  virtual void compile() const override {
    printf("ElseIf \n");
    for(int i=0; i<elsif_stmt_bodies.size(); i++){
        elsif_conds[i]->compile();
        elsif_stmt_bodies[i]->compile();
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

  virtual void printOn(std::ostream &out) const override {
    
    if(!else_stmt.empty()){
      out << "Else (" << *else_stmt[0] << ")\n";
    }
  }
  virtual void compile() const override {
    printf("Else \n");
    else_stmt[0]->compile();
  }

private:
  std::vector<StmtBody *> else_stmt;
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
    out << "For(" << *simple_list1 << ", " << *expr << "," << *simple_list2 << ", " << *stmt_body << ")\n";
  }
  virtual void compile() const override {
    printf("For(");
    simple_list1->compile();
    expr->compile();
    simple_list2->compile();
    stmt_body->compile();
    printf(")\n");
  }
private:
  SimpleList *simple_list1;
  Expr *expr;
  SimpleList *simple_list2;
  StmtBody *stmt_body;
};

// Done
class SimpleList: public AST {
public:
  SimpleList(): simples() {}
  ~SimpleList() { for (Simple *s : simples) delete s; }

  void append(Simple *s) { simples.push_back(s); }
  void insert_front(Simple *s) { simples.insert(simples.begin(), s);}

  virtual void printOn(std::ostream &out) const override {
    out << "SimplesList(";
    bool first = true;
    for (Simple *s : simples) {
      if (!first) out << ", ";
      first = false;
      out << *s;
    }
    out << ")\n";
  }
  virtual void compile() const override {
    printf("SimpleList and these are my stitches: \n");
    for (Simple *s : simples)
      s->compile();
  }
  std::vector<Simple *> get_simples_list(){
    return simples;
  }
private:
  std::vector<Simple *> simples;
};

// Done
class Assign: public Simple{
public:
  Assign(Atom *a, Expr *e): atom(a), expr(e) {}
  virtual void printOn(std::ostream &out) const override {
    out << "assign" << *atom << "," << *expr << "\n";
  }
  virtual void compile() const override {
    printf("Assign\n");
    atom->compile();
    expr->compile();
  }
private:
  Expr *expr;
  Atom *atom;
};

// Done
class Skip: public Simple{
public:
  Skip() {}
  virtual void printOn(std::ostream &out) const override {
    out << "skip\n";
  }
  virtual void compile() const override {
    printf("Skip\n");
  }
};

//TODO : Fix weird type issue on printOn
class FunctionCall: public Simple, public Atom {
public:
  FunctionCall(Id *n): name(n), hasParams(false) {}
  FunctionCall(Id *n, ExprList *el): name(n), params(el), hasParams(true) {}
  //~Call(){}
  virtual void printOn(std::ostream &out) const override {
    if(!hasParams)
      out << "Function call (" << *name << ")\n";
    else
      out << "Function call (" << *name << "," << *params << ")\n";
  }
  virtual void compile() const override {

    printf("Function Call(\n");
    name->compile();
    if(hasParams){
      printf(", ");
      params->compile();
    }
    printf(")\n");
  }
bool hasParams;
private:
  Id *name;
  ExprList *params;
};


class ExprList: public AST {
public:
  ExprList(): expressions() {}
  ~ExprList() { for (Expr *e : expressions) delete e; }

  void append(Expr *e) { expressions.push_back(e); }
  void insert_front(Expr *e) { expressions.insert(expressions.begin(), e);}

  virtual void printOn(std::ostream &out) const override {
    out << "ExprList(";
    bool first = true;
    for (Expr *e : expressions) {
      if (!first) out << ", ";
      first = false;
      out << *e;
    }
    out << ")\n";
  }
  virtual void compile() const override {
    printf("ExprList and these are my stitches: \n");
    for (Expr *e : expressions)
      e->compile();
  }
  std::vector<Expr*> get_expr_list(){
    return expressions;
  }
private:
  std::vector<Expr*> expressions;
};

// Done
class Array: public Atom {
public:
  Array(Atom *a, Expr *e): atom(a), expr(e) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Array (" << *atom << " " << *expr << ")\n";
  }
  virtual void compile() const override {

    printf("i am arraying huehue\n");
    atom->compile();
    expr->compile();

  }
private:
  Atom *atom;
  Expr *expr;
};

// Done
class StringLiteral: public Atom {
public:
  StringLiteral(std::string str): strlit(str) {}
  virtual void printOn(std::ostream &out) const override {
    out << "String" << strlit << ")";
  }
  virtual void compile() const override {
    /*Εδώ θα μπει ο ενδιάμεσος κώδικας μετά*/
    printf("string_literal : %s\n", strlit);
  }
private:
  std::string strlit;
};


// Done
class Id: public Atom {
public:
  Id(std::string v): var(v) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Id(" << var << ")";
  }
  virtual void compile() const override {
    printf("i smell a variable\n");    
  }
private:
  std::string var;
};

// Done
class CharConst: public RValue {
public:
  CharConst(char c): char_const(c) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Char " << char_const << ")";
  }
  virtual void compile() const override {
    /*Εδώ θα μπει ο ενδιάμεσος κώδικας μετά*/
    printf("char const : %c\n", char_const);
  }
private:
  char char_const;
};

// Done
class IntConst: public RValue {
public:
  IntConst(int n): num(n) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Const(" << num << ")";
  }
  virtual void compile() const override {
    /*Εδώ θα μπει ο ενδιάμεσος κώδικας μετά*/
    printf("int_const : %d\n", num);
  }
private:
  int num;
};

// Done
class New: public RValue {
public:
  New(Type *t, Expr *right): typ(t), expr(right){}
  ~New() {delete expr;}
  virtual void printOn(std::ostream &out) const override {
    out << "new (" << *typ << ", " << *expr << ")";
  }
  virtual void compile() const override {
    printf("New \n");
    typ->compile();
    expr->compile();
  }
private:
  Type *typ;
  Expr *expr;
};



class Nil: public RValue {
public:
  Nil() {}
  virtual void printOn(std::ostream &out) const override {
    out << "Nil()";
  }
  virtual void compile() const override {
    printf("Nil\n");
  }
};

// Done
class Boolean: public RValue {
public:
  Boolean(std::string b): boo(b) {} //wink wink pap
  virtual void printOn(std::ostream &out) const override {
    out << "Bool: " << boo << ")";
  }
  virtual void compile() const override {
    if (boo == "true")
      printf("true\n");
    else if (boo == "false")
      printf("false\n");
    else
      printf("Wrong bool value dummy");
  }
private:
  std::string boo;
};

// Done
class BinOp: public RValue {
public:
  BinOp(Expr *l, std::string o, Expr *r): left(l), op(o), right(r) {}
  ~BinOp() { delete left; delete right; }
  virtual void printOn(std::ostream &out) const override {
    out << op << "(" << *left << ", " << *right << ")";
  }
  virtual void compile() const override {
    left->compile();
    right->compile();
    /*Στην θέση των παρακάτω θα μπει ο ενδιάμεσος κώδικας μετά*/
    if(op == "+")
      printf("operator +\n");
    else if(op == "-")
      printf("operator -\n");
    else if(op == "*")
      printf("operator *\n");
    else if(op == "/")
      printf("operator /\n");
    else if(op == "mod")
      printf("operator mod\n");
    else if(op == "=")
      printf("operator =\n");
    else if(op == ">")
      printf("operator >\n");
    else if(op == "<")
      printf("operator <\n");
    else if(op == ">=")
      printf("operator >=\n");
    else if(op == "<=")
      printf("operator <=\n");
    else if(op == "<>")
      printf("operator <>\n");
    else if(op == "and")
      printf("operator and\n");
    else if(op == "or")
      printf("operator or\n");
    else if(op == "#")
      printf("operator #\n");
    else 
      printf("Wrong operator dummy! Huehuehuehue\n");
    }
private:
  Expr *left;
  std::string op;
  Expr *right;
};

// Done
class UnOp: public RValue {
public:
  UnOp(std::string(o), Expr *r): op(o), right(r) {}
  ~UnOp() { delete right; }
  virtual void printOn(std::ostream &out) const override {
    out << op << "( " << *right << ")";
  }
  virtual void compile() const override {
    right->compile();
    if(op == "+")
      printf("operator +\n");
    else if(op == "-")
      printf("operator -\n");
    else if(op == "nil?")
      printf("operator nil?\n");
    else if(op == "not")
      printf("operator not\n");
    else if(op == "head")
      printf("operator head\n");
    else if(op == "tail")
      printf("operator tail\n");
    else 
      printf("Wrong unary operator dummy! Huehuehuehue\n");
    }
private:
  std::string op;
  Expr *right;
};
















// Below here not done






















// Done
class StmtBody: public AST {
public:
  StmtBody(): stmts() {}
  ~StmtBody() { for (Stmt *s : stmts) delete s; }
  void append(Stmt* stmt) {
    stmts.push_back(stmt);
    return;
  }
  virtual void printOn(std::ostream &out) const override {
    out << "StmtBody(";
    for (Stmt *s : stmts) {
      out << *s << ", " ;
    }
    out << "endStmtBody)";
  }
  virtual void compile() const override {
    printf("Stmt body, check these out : \n");
    for (Stmt *fwtakaros : stmts)
      fwtakaros->compile();
  }
  
private:
  std::vector<Stmt*> stmts;
};












// Done
class Exit: public Stmt{
public:
  Exit() {}
  virtual void printOn(std::ostream &out) const override {
    out << "Exit() ";
  }
  virtual void compile() const override {
    printf("exit\n");
  }
};

// Done
class Return: public Stmt{
public:
  Return(Expr* e): ret_expr(e) {}
private: 
virtual void printOn(std::ostream &out) const override {
    out << "Return( " << *ret_expr << ")";
  }
  virtual void compile() const override {
    printf("Return(), expression = \n");
    ret_expr->compile();
  }
  Expr* ret_expr;
};



// Done
class Let: public Stmt {
public:
  Let(Expr *a, Expr *e): atom(a), expr(e) {}
  ~Let() { 
    delete atom;
    delete expr; }
  virtual void printOn(std::ostream &out) const override {
    out << "Let(" << "kkdjgrrjgsrgjsnrgj" << " = " << *expr << ")";
  }
  virtual void compile() const override {
    atom->compile();
    expr->compile();
    printf("Let\n");
  }
private:
  Expr *atom;
  Expr *expr;
};




// Done


// Done
class VarList: public AST {
public:
  VarList(): ids() {}
  void append(Expr * id) {
    ids.push_back(id);
  }

  virtual void printOn(std::ostream &out) const override {
    out << "VarList(";
    bool first = true;
    for (Expr * i : ids) {
      if (!first) out << ", ";
      first = false;
      out << i;
    }
    out << ")";
  }
  virtual void compile() const override {
    printf("VarList and these are my variantes: \n");
    for (Expr * e : ids)
      e->compile(); //edw tha prepe na nai kana COMPILE, lew gw twra
    }
private:
  std::vector<Expr *> ids;
};

// Done
class Print: public Stmt {
public:
  Print(Expr *e): expr(e) {}
  ~Print() { delete expr; }
  virtual void printOn(std::ostream &out) const override {
    out << "Print(" << *expr << ")";
  }
  virtual void compile() const override {
    printf("  pushl %%edi\n");
    expr->compile();
    printf("  subl $8, %%esp\n");
    printf("  call _writeInteger\n");
    printf("  addl $12, %%esp\n");
    printf("  movl $NL, %%eax\n");
    printf("  pushl %%eax\n");
    printf("  subl $8, %%esp\n");
    printf("  call _writeString\n");
    printf("  addl $12, %%esp\n");
    printf("  popl %%edi\n");
  }
private:
  Expr *expr;
};

// TODO : Dex xerw ti paizei



// TODO : Kati exeis kanei de to sbhnw





















#endif