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


//types για μη τερματικά σύμβολα
%type<ast> Call Stmt_Body Stmt_Elsif_Body
%type<expr> Expr Atom
%type<stmt> Stmt Simple
%type<simple_list> Simple_List Simple_Comma
%type<expr_list> Expr_Comma


%%

Program:
    Func_def
;

Func_def:
    "def" Header ":" Func_def_dec  Stmt_Body "end"
;

Func_def_dec:
    Func_def_dec Func_def
|   Func_def_dec Func_decl
|   Func_def_dec Var_def
|   /*ε*/
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
    Simple  { $$ = $1; }
|   "exit"
|   "return" Expr   { $$ = new Return($2); }
|   "if" Expr ":" Stmt_Body Stmt_Elsif_Body "end"   { $$ = new If($2, $4, $5); }
|   "if" Expr ":" Stmt_Body Stmt_Elsif_Body "else" ":" Stmt_Body "end"  { $$ = new If($2, $4, $5, $8); }
|   "for" Simple_List ";" Expr ";" Simple_List ":" Stmt_Body "end"
;

Stmt_Body:
    Stmt    { $$ = new StmtBody(); }
|   Stmt_Body Stmt  { $1->append($2); $$ = $1 } 
;

Stmt_Elsif_Body:
    /*ε*/  { $$ = new Elsif(); }
|   "elsif" Expr ":" Stmt_Body Stmt_Elsif_Body  { $5->append($2, $4); $$ = $5; } /* ισως χρειαστει inverse */


Simple:
    "skip"          { $$ = new Skip(); }
|   Atom ":=" Expr  { $$ = new Let($1,$3); }
|   Call            { $$ = $1; }
;

Simple_List:
    Simple                  { $$ = new SimpleList($1); }
|   Simple  Simple_Comma    { $2->insert_front($1); $$ = $2; }
;

/* Υλοποιεί το ("," Simple)* της γραμματικής.
Το SimpleList() κατασκευάζει μια κενή λίστα από Simples η οποία
θα γίνει append. */
Simple_Comma:
    /*ε*/  { $$ = new SimpleList(); }
|   Simple_Comma "," Simple { $1->append($3); $$ = $1; }
;

Call:
    id  "("")"                  { $$ = new FunctionCall($1);}
|   id  "(" Expr Expr-Comma ")" { $4->insert_front($3); $$ = new FunctionCall($1, $4); }
;

Expr-Comma:
    /*ε*/               { $$ = new ExprList(); }
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
