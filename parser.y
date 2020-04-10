%{
#include <cstdio>
#include <string.h>
#include "lexer.hpp"

std::map<char, int> globals;
%}

%union {
  int num;
  char c;
  bool b;
  char op[4];
  char sep[2];
  char name[80]; //!!
  char str[80]; //!!
}

%token<name> T_id "id"
%token<op> T_and "and"
%token T_bool "bool"
%token T_char "char"
%token T_decl "decl"
%token T_def "def"
%token T_else "else"
%token T_elseif "elseif"
%token T_end "end"
%token T_exit "exit"
%token<b> T_false "false"
%token T_for "for"
%token T_head "head"
%token T_if "if"
%token T_int "int"
%token T_list "list"
%token<op> T_mod "mod"
%token T_new "new"
%token T_nil "nil"
%token T_nil2 "nil?"
%token<op> T_not "not"
%token<op> T_or "or"
%token T_ref "ref"
%token T_return "return"
%token T_skip "skip"
%token T_tail "tail"
%token<b> T_true "true"
%token<num> T_const "int_const"
%token<c> T_singlechar  "char_const"
%token<str> T_string    "string_literal"
%token<op> T_operator   "operator"
%token<sep> T_seperator "seperator"


%left<op> 'or' 
%left<op> 'and'
%nonassoc<op> 'not'

%nonassoc<op> '=' '<>' '<=' '>=' '<' '>'

%right<op> '#'

%left<op> '+' '-'
%left<op> '*' '/' 'mod'

%left<op> UMINUS UPLUS

%%

Program:
    Funcdef;

Func_def:
    "def" Header ":" Func_def_dec  Stmt_body "end";

Func_def_dec:
    Func_def_dec Func_def
|   Func_def_dec Func_decl
|   Func_def_dec Var_def
|   /*ε*/;

Stmt_body:
    Stmt
|   Stmt_body Stmt;

Header:
    Type id '('')'
|   Type id '(' Formal Par ')'
|   id '('')'
|   id '(' Formal Par ')';


Par:
|   Par ';' Formal;
|   /*e*/;

Formal:
    "ref" Type id Var_Comma
|   Type id Var_Comma;

Var_Comma:
    /* e*/
|   Var_Comma , id;

Type:
    "int" | "char" | "bool" | Type '['']' | "list" '['Type']';

Func_Decl:
    "decl" Header;

Var_Def:
    Type id
|   Type id Var_Comma;

Stmt:
    Simple
|   "exit"
|   "return" Expr
|   "if" Expr ":" Stmt_body Stmt_Else_body "end"
|   "if" Expr ":" Stmt_body Stmt_Else_body "else" ":" Stmt_body "end"
|   "for" Simple_List ";" Expr ";" Simple_List ":" Stmt_body "end";

Simple:
    "skip"
|   Atom ":=" Expr
|   Call;

Simple_List:
    Simple
|   Simple  Simple_Comma;

Simple_Comma:
    /*ε*/
|   Simple_Comma "," Simple;

Call:
    id  "("")"
|   id  "(" Expr Expr-Comma ")";

Expr-Comma:
    /*ε*/
|   Expr_Commaprintf("  subl %%ebx, %%eax\n");
        printf("  pushl %%eax\n"); "," Expr;


Atom:
    id  |   string_literal  |   Atom "[" Expr "]" | Call;


Expr:
    Atom
|   int_const
|   string_literal
|   char_const
|   "(" Expr ")"
|   Expr "+" Expr
|   Expr "-" Expr
|   Expr "*" Expr 
|   Expr "/" Expr
|   Expr "mod" Expr
|   Expr "=" Expr
|   Expr "<>" Expr
|   Expr "<" Expr 
|   Expr ">" Expr
|   Expr "<=" Expr
|   Expr ">=" Expr
|   "-" Expr %prec UMINUS
|   "+" Expr %prec UPLUS
|   "true"
|   "false"
|   "not" Expr
|   Expr "and" Expr
|   Expr "or" Expr
|   "new" Type "[" Expr "]"
|   "nil"
|   "nil?" "(" Expr ")"
|   Expr "#" Expr
|   "head" "(" Expr ")"
|   "tail" "(" Expr ")";


%%

int main(){
  return 0;
}