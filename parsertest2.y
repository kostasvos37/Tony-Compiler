%{
#include <cstdio>
#include <string.h>
#include "lexer.hpp"
#include "ast.hpp"

%}

/*Ορίζουμε τους σημασιολογικούς τύπους των τερματικών που φαίνονται στα tokens παρακάτω. 
πχ: στο ' %token<op> T_and "and" ' λέμε ότι το τερματικό "and" έχει τύπο op (που έχουμε ορίσει
στο union) ο οποίος πρακτικά είναι ένας πίνακας από characters.

Επίσης, ορίζουμε και τους σημασιολογικούς τύπους των μη τερματικών συμβόλων όπως το Expr
το Stmt κλπ.*/
%union {
    Expr *expr;
    Stmt *stmt;
    int num;
    char c;
    bool b;
    char op[4];
    char sep[2];
    char name[80]; //!!
    char str[80]; //!!
}

%token<name> T_id       "id"
%token<op> T_and        "and"
%token<op> T_mod        "mod"
%token<op> T_not        "not"
%token<op> T_or         "or"
%token<op> T_operator   "operator"
%token<sep> T_seperator "seperator"
%token<b> T_true        "true"
%token<b> T_false       "false"
%token<num> T_const     "int_const"
%token<str> T_string    "string_literal"
%token<c> T_singlechar  "char_const"
%token T_bool           "bool"
%token T_char           "char"
%token T_decl           "decl"
%token T_def            "def"
%token T_else           "else"
%token T_elseif         "elseif"
%token T_end            "end"
%token T_exit           "exit"
%token T_for            "for"
%token T_head           "head"
%token T_if             "if"
%token T_int            "int"
%token T_list           "list"
%token T_new            "new"
%token T_nil            "nil"
%token T_nil2           "nil?"
%token T_ref            "ref"
%token T_return         "return"
%token T_skip           "skip"
%token T_tail           "tail"


%left<op> 'or' 
%left<op> 'and'
%nonassoc<op> 'not'
%nonassoc<op> '=' '<>' '<=' '>=' '<' '>'
%right<op> '#'
%left<op> '+' '-'
%left<op> '*' '/' 'mod'
%left<op> UMINUS UPLUS


//types
%type<expr> Expr Atom


%%

Program:
    Funcdef
;

Func_def:
    "def" Header ":" Func_def_dec  Stmt_body "end"
;

Func_def_dec:
    Func_def_dec Func_def
|   Func_def_dec Func_decl
|   Func_def_dec Var_def
|   /*ε*/
;

Stmt_body:
    Stmt
|   Stmt_body Stmt
;

Header:
    Type id '('')'
|   Type id '(' Formal Par ')'
|   id '('')'
|   id '(' Formal Par ')'
;


Par:
|   Par ';' Formal;
|   /*e*/
;

Formal:
    "ref" Type id Var_Comma
|   Type id Var_Comma
;

Var_Comma:
    /* e*/
|   Var_Comma , id
;

Type:
    "int" 
|   "char" 
|   "bool" 
|   Type '['']' 
|   "list" '['Type']'
;

Func_Decl:
    "decl" Header
;

Var_Def:
    Type id
|   Type id Var_Comma
;

Stmt:
    Simple
|   "exit"
|   "return" Expr
|   "if" Expr ":" Stmt_body Stmt_Else_body "end"
|   "if" Expr ":" Stmt_body Stmt_Else_body "else" ":" Stmt_body "end"
|   "for" Simple_List ";" Expr ";" Simple_List ":" Stmt_body "end"
;

Simple:
    "skip"          {$$ = new Skip();}
|   Atom ":=" Expr  {$$ = new Let($1,$3);}
|   Call            {$$ = $1;}
;

Simple_List:
    Simple                  { $$ = new SimpleList($1); }
|   Simple  Simple_Comma    { $1->append($3); $$ = $1; }
;

Simple_Comma:
    /*ε*/  { $$ = new SimpleList(); }
|   Simple_Comma "," Simple { $1->append($3); $$ = $1; }
;

Call:
    id  "("")" { $$ = new FunctionCall($1);}
|   id  "(" Expr Expr-Comma ")" { $4->append($3); $4->setName($1); $$ = $4 }
;

Expr-Comma:
    /*ε*/ { $$ = new FunctionCall(); }
|   Expr_Comma "," Expr { $1->append($3); $$ = $1; }
;

Atom:
    id                 { $$ = new Id($1); }
|   string_literal     { $$ = new StringLiteral($1); }
|   Atom "[" Expr "]"  { $$ = new Array($1, $3); }
|   Call               { $$ = $1; } 
;

Expr:
    Atom             { $$ = $1; }
|   int_const        { $$ = new IntConst($1); }
|   char_const       { $$ = new CharConst($1); }
|   "(" Expr ")"     { $$ = $2; }
|   Expr "+" Expr    { $$ = new BinOp($1, $2, $3); }
|   Expr "-" Expr    { $$ = new BinOp($1, $2, $3); }
|   Expr "*" Expr    { $$ = new BinOp($1, $2, $3); }
|   Expr "/" Expr    { $$ = new BinOp($1, $2, $3); }
|   Expr "mod" Expr  { $$ = new BinOp($1, $2, $3); }
|   Expr "=" Expr    { $$ = new BinOp($1, $2, $3); }
|   Expr "<>" Expr   { $$ = new BinOp($1, $2, $3); }
|   Expr "<" Expr    { $$ = new BinOp($1, $2, $3); }
|   Expr ">" Expr    { $$ = new BinOp($1, $2, $3); }
|   Expr "<=" Expr   { $$ = new BinOp($1, $2, $3); }
|   Expr ">=" Expr   { $$ = new BinOp($1, $2, $3); }
|   Expr "and" Expr  { $$ = new BinOp($1, $2, $3); }
|   Expr "or" Expr   { $$ = new BinOp($1, $2, $3); }
|   Expr "#" Expr    { $$ = new BinOp($1, $2, $3); }
|   "not" Expr              { $$ = new UnOp($1, $2); }
|   "-" Expr %prec UMINUS   { $$ = new UnOp($1, $2); }
|   "+" Expr %prec UPLUS    { $$ = new UnOp($1, $2); }
|   "nil?" "(" Expr ")"     { $$ = new UnOp($1, $3); }
|   "head" "(" Expr ")"     { $$ = new UnOp($1, $3); }
|   "tail" "(" Expr ")"     { $$ = new UnOp($1, $3); }
|   "true"             { $$ = new Boolean($1); }
|   "false"            { $$ = new Boolean($1); }
|   "new" Type "[" Expr "]" { $$ = new New($2, $4); }
|   "nil" {}
;

%%

int main(){
  return 0;
}