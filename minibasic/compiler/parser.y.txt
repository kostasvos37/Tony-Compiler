%{
#include <cstdio>
#include "ast.hpp"
#include "lexer.hpp"

std::map<char, int> globals;
%}

%token T_for "for"
%token T_print "print"
%token T_let "let"
%token T_if "if"
%token T_then "then"
%token T_else "else"
%token T_do "do"
%token T_begin "begin"
%token T_end "end"
%token<var> T_id
%token<num> T_const

%left<op> '+' '-'
%left<op> '*' '/' '%'

%expect 1

%union {
  Block *block;
  Stmt *stmt;
  Expr *expr;
  char var;
  int num;
  char op;
}

%type<block> program stmt_list
%type<stmt>  stmt
%type<expr>  expr

%%

program:
  stmt_list {
    //std::cout << "AST: " << *$1 << std::endl;
    $1->compile();
  }
;

stmt_list:
  /* nothing */ { $$ = new Block(); }
| stmt_list stmt { $1->append($2); $$ = $1; }
;

stmt:
  "let" T_id '=' expr { $$ = new Let($2, $4); }
| "print" expr { $$ = new Print($2); }
| "for" expr "do" stmt { $$ = new For($2, $4); }
| "if" expr "then" stmt { $$ = new If($2, $4); }
| "if" expr "then" stmt "else" stmt { $$ = new If($2, $4, $6); }
| "begin" stmt_list "end" { $$ = $2; }
;

expr:
  T_id { $$ = new Id($1); }
| T_const { $$ = new Const($1); }
| '(' expr ')' { $$ = $2; }
| expr '+' expr { $$ = new BinOp($1, $2, $3); }
| expr '-' expr { $$ = new BinOp($1, $2, $3); }
| expr '*' expr { $$ = new BinOp($1, $2, $3); }
| expr '/' expr { $$ = new BinOp($1, $2, $3); }
| expr '%' expr { $$ = new BinOp($1, $2, $3); }
;

%%

int main() {
  prologue();
  int result = yyparse();
  epilogue();
  // if (result == 0) printf("Success.\n");
  return result;
}
