%{
#include <cstdio>
#include <string.h>
#include "lexer.hpp"
#include "ast.hpp"

%}

/*Ορίζουμε τους σημασιολογικούς τύπους των τερματικών που φαίνονται στα tokens παρακάτω. 
πχ: στο ' %token<op> T_and "and" ' λέμε ότι το τερματικό "and" έχει τύπο op (που έχουμε ορίσει
στο union) ο οποίος πρακτικά είναι ένας πίνακας από characters.

Επίσης, ορίζουμε και τους σημασιολογικούς τύπους των μη τερματικών συμβόλων όπως το Expr,
το Stmt κλπ.*/
%union {
    AST *ast;
    Expr *expr;
    Stmt *stmt;
    SimpleList *simple_list;
    ExprList *expr_list;
    VarList *var_list;
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
%token T_elseif         "elsif"
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


//types για μη τερματικά σύμβολα
%type<ast> Call Stmt_Body Stmt_Elsif_Body
%type<expr> Expr Atom
%type<stmt> Stmt Simple
%type<simple_list> Simple_List Simple_Comma
%type<expr_list> Expr_Comma
%type<var_list> Var_Comma


%%

/*=============================================
    Function Definitions
==============================================*/

Program:
    Func_def
;

Func_def:
    "def" Header ":" Def_list  Stmt_Body "end"
;

Func_def_dec:
    Func_def Def_list
|   Func_decl Def_list 
|   Var_def Def_list
|   /*ε*/
;

Header:
    Type T_id '('')'
|   Type T_id '(' Formal Par ')'
|   T_id '('')'
|   T_id '(' Formal Par ')'
;


Par:
|   ';' Formal Par;
|   /*e*/
;

Formal:
    "ref" Type T_id Var_Comma
|   Type T_id Var_Comma
;


Var_Comma:
    /* e*/  { /*$$ = new VarList(); */}
|   ',' T_id Var_Comma { /*$1->append($3); $$ = $1; */}
;

Type:
    "int" 
|   "char" 
|   "bool" 
|   Type '[' ']' 
|   "list" '[' Type ']'
;

Func_Decl:
    "decl" Header
;

Var_Def:  Type T_id Var_Comma 

/* 
Stmt:
    Simple  { $$ = $1; }
|   "exit"  { $$ = new Exit(); }
|   "return" Expr   { $$ = new Return($2); }
|   "if" Expr ":" Stmt_Body Stmt_Elsif_Body "end"   { $$ = new If($2, $4, $5); }
|   "if" Expr ":" Stmt_Body Stmt_Elsif_Body "else" ":" Stmt_Body "end"  { $$ = new If($2, $4, $5, $8); }
|   "for" Simple_List ";" Expr ";" Simple_List ":" Stmt_Body "end"  { $$ = new For($2, $4, $6, $8); }
; */

Stmt:
    Simple  
|   "exit"  
|   "return" Expr   
|   If_Clause
|   For_Clause
;

Stmt_Body:  Stmt    { $$ = new StmtBody(); } Stmt_Full
;

Stmt_Full:   Stmt Stmt_Full   { $1->append($2); $$ = $1 }
| /*e*/
; 
If_Clause   :
    "if" Expr ':' Elsif_Clause Else_Clause "end";


Elsif_Clause : "elsif" Expr ':' Stmt_Body Elsif_Clause
| /*e*/
;

Else_Clause: "else" ':' Stmt_Body
| /*e*/
;

For_Clause: "for" Simple_List ';' Expr ';' Simple_List ':' Stmt_Body "end"
;

Simple:
    "skip"          { /* $$ = new Skip();  */}
|   Atom ":=" Expr  { /* $$ = new Let($1,$3); */ }
|   Call            { /* $$ = $1; */ }
;


Simple_List: Simple  Simple_Comma    { /* $2->insert_front($1); $$ = $2; */ }
;



/* Υλοποιεί το ("," Simple)* της γραμματικής.
Το SimpleList() κατασκευάζει μια κενή λίστα από Simples η οποία
θα γίνει append. */
Simple_Comma:
    /*ε*/  { /* $$ = new SimpleList(); */ }
|   ',' Simple Simple_Comma { /* 1->append($3); $$ = $1; */ }
;

Call:
    T_id  '(' ')'                  { /* $$ = new FunctionCall($1); */}
|   T_id  '('' ExprList ')'   { /* $4->insert_front($3); $$ = new FunctionCall($1, $4); */ }
;

Expr_List: Expr  Expr_Comma    { /* $2->insert_front($1); $$ = $2; */ }
;

Expr_Comma:
    /*ε*/               { /* $$ = new ExprList(); */ }
|   ',' Expr Expr_Comma { /* $1->append($3); $$ = $1; */ }
;

Atom:
    T_id                 { $$ = new Id($1); }
|   T_string     { $$ = new StringLiteral($1); }
|   Atom '[' Expr ']' { $$ = new Array($1, $3); }
|   Call               { $$ = $1; } 
;

Expr:
    Atom             { $$ = $1; }
|   Value
;

Value:
        T_const        { $$ = new IntConst($1); }
    |   T_singlechar       { $$ = new CharConst($1); }
    |   '(' Expr ')'     { $$ = $2; }
    |   '+' Expr    { $$ = new UnOp($1, $2); }
    |   '-' Expr    { $$ = new UnOp($1, $2); }
    |   Expr '+' Expr    { $$ = new BinOp($1, $2, $3); }
    |   Expr '-' Expr    { $$ = new BinOp($1, $2, $3); }
    |   Expr '*' Expr    { $$ = new BinOp($1, $2, $3); }
    |   Expr '/' Expr    { $$ = new BinOp($1, $2, $3); }
    |   Expr "mod" Expr  { $$ = new BinOp($1, $2, $3); }
    |   Expr '=' Expr    { $$ = new BinOp($1, $2, $3); }
    |   Expr "<>" Expr   { $$ = new BinOp($1, $2, $3); }
    |   Expr '<' Expr    { $$ = new BinOp($1, $2, $3); }
    |   Expr '>' Expr    { $$ = new BinOp($1, $2, $3); }
    |   Expr "<=" Expr   { $$ = new BinOp($1, $2, $3); }
    |   Expr ">=" Expr   { $$ = new BinOp($1, $2, $3); }
    |   "not" Expr              { $$ = new UnOp($1, $2); }
    |   Expr "and" Expr  { $$ = new BinOp($1, $2, $3); }
    |   Expr "or" Expr   { $$ = new BinOp($1, $2, $3); }
    |   "true"         { $$ = new Boolean($1); }
    |   "false"            { $$ = new Boolean($1); }
    |   "new" Type '[' Expr ']' { $$ = new New($2, $4); }
    |   Expr '#' Expr    { $$ = new BinOp($1, $2, $3); }
    |   "nil" {}
    |   "nil?" '(' Expr ')'     { $$ = new UnOp($1, $3); }
    |   "head" '(' Expr ')'     { $$ = new UnOp($1, $3); }
    |   "tail" '(' Expr ')'     { $$ = new UnOp($1, $3); }
    ;

%%

int main(){
  return 0;
}
