#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>

class AST {
public:
  virtual ~AST() {}
  virtual void printOn(std::ostream &out) const = 0;
};

//not sure what this does
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

extern std::map<char, int> globals;

class Id: public Expr {
public:
  Id(char *v): var(std::string(v)) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Id(" << var << ")";
  }
  virtual void compile() const override {
    printf("i smell a variable\n");    
    //return globals[var];
  }
private:
  std::string var;
};

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

class IntConst: public Expr {
public:
  IntConst(int n): num(n) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Const(" << num << ")";
  }
  virtual void compile() const override {
    printf("int_const == %d\n", num);
  }
private:
  int num;
};

class CharConst: public Expr {
public:
  CharConst(char c): char_const(c) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Char " << char_const << ")";
  }
  virtual void compile() const override {
    printf("string_literal == %c\n", char_const);
  }
private:
  char char_const;
};

class StringLiteral: public Expr {
public:
  StringLiteral(char str[]): strlit(str) {}
  virtual void printOn(std::ostream &out) const override {
    out << "String" << " whateva " << ")";
  }
  virtual void compile() const override {
    printf("string_literal == %c\n", strlit[0]);
  }
private:
  char *strlit;
};

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

class New: public Expr {
public:
  New(std::string t, Expr *right): typ(t), expr(right){}
  ~New() {delete expr;}
  virtual void printOn(std::ostream &out) const override {
    out << typ;
  }
  virtual void compile() const override {
    expr->compile();
    printf("New %s\n", typ);
  }
private:
  std::string typ;
  Expr *expr;
};



class Let: public Stmt {
public:
  Let(char v, Expr *e): var(v), expr(e) {}
  ~Let() { delete expr; }
  virtual void printOn(std::ostream &out) const override {
    out << "Let(" << var << " = " << *expr << ")";
  }
  virtual void compile() const override {
    //globals[var] = expr->compile(); will need fixing
  }
private:
  char var;
  Expr *expr;
};

class FunctionCall: public Expr {
public:
  FunctionCall(): parameters() {}
  FunctionCall(char *n): name(std::string(n)), parameters() {}
  ~FunctionCall() { for (Expr *e : parameters) delete e; }
  void append(Expr *e) { parameters.push_back(e); }
  void setName(char *n) { name = std::string(name);}
  virtual void printOn(std::ostream &out) const override {
    out << "FunctionCall(";
    bool first = true;
    for (Expr *e : parameters) {
      if (!first) out << ", ";
      first = false;
      out << *e;
    }
    out << ")";
  }
  virtual void compile() const override {
    printf("Function call and these are my bitches: \n");
    for (Expr *e : parameters)
      e->compile();
  }
private:
  std::string name;
  std::vector<Expr*> parameters;
};

//MEXRI EDW

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

class If: public Stmt {
public:
  If(Expr *c, Stmt *s1, Stmt *s2 = nullptr):
    cond(c), stmt1(s1), stmt2(s2) {}
  ~If() { delete cond; delete stmt1; delete stmt2; }
  virtual void printOn(std::ostream &out) const override {
    out << "If(" << *cond << ", " << *stmt1;
    if (stmt2 != nullptr) out << ", " << *stmt2;
    out << ")";
  }
  virtual void compile() const override {
    static int counter = 0;
    cond->compile();
    printf("  popl %%eax\n");
    printf("  andl %%eax, %%eax\n");
    int l_false = counter++;
    printf("  jz Lif%d\n", l_false);
    stmt1->compile();
    int l_end = counter++;
    printf("  jmp Lif%d\n", l_end);
    printf("Lif%d:\n", l_false);
    if (stmt2 != nullptr)
      stmt2->compile();
    printf("Lif%d:\n", l_end);
  }
private:
  Expr *cond;
  Stmt *stmt1;
  Stmt *stmt2;
};

class For: public Stmt {
public:
  For(Expr *e, Stmt *s): expr(e), stmt(s) {}
  ~For() { delete expr; delete stmt; }
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
  Expr *expr;
  Stmt *stmt;
};

class Block: public Stmt {
public:
  Block(): stmt_list() {}
  ~Block() { for (Stmt *s : stmt_list) delete s; }
  void append(Stmt *s) { stmt_list.push_back(s); }
  virtual void printOn(std::ostream &out) const override {
    out << "Block(";
    bool first = true;
    for (Stmt *s : stmt_list) {
      if (!first) out << ", ";
      first = false;
      out << *s;
    }
    out << ")";
  }
  virtual void compile() const override {
    for (Stmt *s : stmt_list)
      s->compile();
  }
private:
  std::vector<Stmt *> stmt_list;
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