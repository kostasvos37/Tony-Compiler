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
class Id: public Expr {
public:
  Id(char *v): var(std::string(v)) {}
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
class Array: public Expr {
public:
  Array(char *t): typ(std::string(t)) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Array " << typ << " " << *expr << "\n";
  }
  virtual void compile() const override {
    expr->compile();
    printf("i am arraying huehue\n");
  }
private:
  std::string typ;
  Expr *expr;
};

// Done
class IntConst: public Expr {
public:
  IntConst(int n): num(n) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Const(" << num << ")";
  }
  virtual void compile() const override {
    /*Εδώ θα μπει ο ενδιάμεσος κώδικας μετά*/
    printf("int_const == %d\n", num);
  }
private:
  int num;
};

// Done
class CharConst: public Expr {
public:
  CharConst(char c): char_const(c) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Char " << char_const << ")";
  }
  virtual void compile() const override {
    /*Εδώ θα μπει ο ενδιάμεσος κώδικας μετά*/
    printf("string_literal == %c\n", char_const);
  }
private:
  char char_const;
};

// Done
class StringLiteral: public Expr {
public:
  StringLiteral(char str[]): strlit(str) {}
  virtual void printOn(std::ostream &out) const override {
    out << "String" << " whateva " << ")";
  }
  virtual void compile() const override {
    /*Εδώ θα μπει ο ενδιάμεσος κώδικας μετά*/
    printf("string_literal == %c\n", strlit[0]);
  }
private:
  char *strlit;
};

// Done
class Boolean: public Expr {
public:
  Boolean(bool b): boo(b) {} //wink wink pap
  virtual void printOn(std::ostream &out) const override {
    out << "Bool: " << boo << ")";
  }
  virtual void compile() const override {
    printf("bool == %d\n", boo);
  }
private:
  bool boo;
};

// Done
class BinOp: public Expr {
public:
  BinOp(Expr *l, char *o, Expr *r): left(l), op(std::string(o)), right(r) {}
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
class UnOp: public Expr {
public:
  UnOp(char *o, Expr *r): op(std::string(o)), right(r) {}
  ~UnOp() { delete right; }
  virtual void printOn(std::ostream &out) const override {
    out << op << " " << *right;
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

// Done
class New: public Expr {
public:
  New(char *t, Expr *right): typ(std::string(t)), expr(right){}
  ~New() {delete expr;}
  virtual void printOn(std::ostream &out) const override {
    out << typ;
  }
  virtual void compile() const override {
    expr->compile();
    printf("New %s\n", typ.c_str());
  }
private:
  std::string typ;
  Expr *expr;
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
class Skip: public Stmt{
public:
  Skip() {}
  virtual void printOn(std::ostream &out) const override {
    out << "skip\n";
  }
  virtual void compile() const override {
    printf("i just skipped\n");
  }
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
class SimpleList: public AST {
public:
  SimpleList(): simples() {}
  SimpleList(Stmt *simp) {
    std::vector<Stmt*> temp;
    simples = temp;
    simples.push_back(simp); 
  }  
  ~SimpleList() { for (Stmt *e : simples) delete e; }
  void append(Stmt *e) { simples.push_back(e); }
  void insert_front(Stmt *e) { simples.insert(simples.begin(), e); }
  virtual void printOn(std::ostream &out) const override {
    out << "SimpleList(";
    bool first = true;
    for (Stmt *e : simples) {
      if (!first) out << ", ";
      first = false;
      out << *e;
    }
    out << ")";
  }
  virtual void compile() const override {
    printf("SimpleList and these are my stitches: \n");
    for (Stmt *e : simples)
      e->compile();
  }
private:
  std::vector<Stmt*> simples; 
};

// Done
class FunctionCall: public AST {
public:
  FunctionCall(char *n): name(std::string(n)), parameters() {}	
  FunctionCall(char *n, std::vector<Expr*> param): name(std::string(n)), parameters(param) {}
  ~FunctionCall() { for (Expr *e : parameters) delete e; }
  virtual void printOn(std::ostream &out) const override {
    out << "FunctionCall(";
    for (Expr *e : parameters) {
      out << *e << ", ";
    }
    out << ")";
  }
  
  virtual void compile() const override {
    printf("Function call and these are my bitches: \n");
    for (Expr *e : parameters) e->compile();
  }
  private:	
  std::string name;	
  std::vector<Expr*> parameters;	
};

// Done
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
    out << ")";
  }
  virtual void compile() const override {
    printf("ExprList and these are my stitches: \n");
    for (Expr *e : expressions)
      e->compile();
  }
private:
  std::vector<Expr*> expressions;
};

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
class If: public Stmt {
public:
  If(Expr *c, StmtBody *s, Elsif *e, Stmt *else_s = nullptr) {
      main_cond = c;
      main_stmt_body = s;
      elsif_conds = e->get_conds();
      elsif_stmt_bodies = e->get_stmt_bodies();
      else_stmt = else_s;
    }
  ~If() {
    delete main_cond;
    delete main_stmt_body;
    for (Expr* e: elsif_conds) delete e;
    for (StmtBody* s: elsif_stmt_bodies) delete s;
    delete else_stmt; }
  void append_elsif(Elsif* elsif) {
    elsif_conds = elsif->get_conds();
    elsif_stmt_bodies = elsif->get_stmt_bodies();
    return;
  }
  virtual void printOn(std::ostream &out) const override {
    out << "If(" << *cond << ", " << *stmt1;
    if (stmt2 != nullptr) out << ", " << *stmt2;
    out << ")";
  }
private:
  Expr *main_cond;
  StmtBody *main_stmt_body;
  std::vector<Expr *> elsif_conds;
  std::vector<StmtBody *> elsif_stmt_bodies;
  Stmt *else_stmt;
};


// TODO : Ta idia
class Elsif: public Stmt {
public:
  Elsif(): elsif_conds(), elsif_stmt_bodies() {}
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
  void append(Expr* e, StmtBody* s) {
    elsif_conds.push_back(e);
    elsif_stmt_bodies.push_back(s);
  }
private:
  std::vector<Expr *> elsif_conds;
  std::vector<StmtBody *> elsif_stmt_bodies;
};

// TODO : Kati exeis kanei de to sbhnw
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
    out << "For(" << *expr << ", " << *stmt << ")";
  }
  virtual void compile() const override {
    static int counter = 0;
    expr->compile();
    int l_loop = counter++;
    printf("Lfor%d:\n", l_loop);
    printf("  popl %%eax\n");
    printf("  cmpl $0, %%eax\n");
    int l_end = counter++;
    printf("  jle Lfor%d\n", l_end);
    printf("  decl %%eax\n");
    printf("  pushl %%eax\n");
    stmt->compile();
    printf("  jmp Lfor%d\n", l_loop);
    printf("Lfor%d:\n", l_end);
  }
private:
  SimpleList *simple_list1;
  Expr *expr;
  SimpleList *simple_list2;
  StmtBody *stmt_body;
};


inline void prologue() {
  printf("\
.text\n\
.global _start\n\
\n\
_start:\n\
  movl $var, %%edi\n\
\n");
}

inline void epilogue() {
  printf("\
\n\
  movl $0, %%ebx\n\
  movl $1, %%eax\n\
  int $0x80\n\
\n\
.data\n\
var:\n\
.rept 26\n\
.long 0\n\
.endr\n\
NL:\n\
.asciz \"\\n\"\n\
");
}

#endif