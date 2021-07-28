%{
#include <cstdio>
#include "ast.hpp"
#include "lexer.hpp"

SymbolTable st;
std::vector<int> rt_stack;
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
%token T_var "var"
%token T_int "int"
%token T_bool "bool"
%token<var> T_id
%token<num> T_const

%nonassoc<op> '=' '<' '>'
%left<op> '+' '-'
%left<op> '*' '/' '%'

%expect 1

%union {
  Block *block;
  Stmt *stmt;
  Expr *expr;
  Decl *decl;
  Type type;
  char var;
  int num;
  char op;
}

%type<block> program block decl_list stmt_list
%type<stmt>  stmt
%type<expr>  expr
%type<decl>  decl
%type<type>  type

%%

program:
  block {
    $1->sem();
    //std::cout << "AST: " << *$1 << std::endl;
    $1->run();
  }
;

block:
  decl_list stmt_list { $1->merge($2); $$ = $1; }
;

decl_list:
  /* nothing */ { $$ = new Block(); }
| decl_list decl { $1->append_decl($2); $$ = $1; }
;

decl:
  "var" T_id ':' type { $$ = new Decl($2, $4); }
;

type:
  "int"  { $$ = TYPE_int; }
| "bool" { $$ = TYPE_bool; }
;

stmt_list:
  /* nothing */ { $$ = new Block(); }
| stmt_list stmt { $1->append_stmt($2); $$ = $1; }
;

stmt:
  "let" T_id '=' expr { $$ = new Let($2, $4); }
| "print" expr { $$ = new Print($2); }
| "for" expr "do" stmt { $$ = new For($2, $4); }
| "if" expr "then" stmt { $$ = new If($2, $4); }
| "if" expr "then" stmt "else" stmt { $$ = new If($2, $4, $6); }
| "begin" block "end" { $$ = $2; }
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
| expr '<' expr { $$ = new BinOp($1, $2, $3); }
| expr '=' expr { $$ = new BinOp($1, $2, $3); }
| expr '>' expr { $$ = new BinOp($1, $2, $3); }
;

%%

int main() {
  int result = yyparse();
  // if (result == 0) printf("Success.\n");
  return result;
}
