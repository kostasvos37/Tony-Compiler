%{

#include <cstdio>
#include <string.h>
#include "lexer.hpp"
#include "ast.hpp"

%}

%union {
  AST *ast;
  int num;
  char c;
  bool b;
  char op[5];
  char sep[3];
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


%left "or"
%left "and"
%nonassoc "not"

%nonassoc "=" "<=" ">=" ">" "<"

%right "#"

%left "+" "-"
%left "*" "/" "mod"

%left UMINUS UPLUS

// TYPES EDW, ta types einai mh termatika kai ti shmasiologiki timh epistrefoun den ta xoyme valei prepei na ta valoume de douleuei alliws a
%type<ast> Program
%type<ast> Func_def
%type<ast> Func_def_dec
%type<ast> Stmt_body
%type<ast> Header
%type<ast> Par
%type<ast> Formal
%type<ast> Var_Comma
%type<ast> Type
%type<ast> Func_Decl
%type<ast> Var_Def
%type<ast> Stmt
%type<ast> Simple
%type<ast> Simple_List
%type<ast> Simple_Comma
%type<ast> Call
%type<ast> Expr_Comma
%type<ast> Atom
%type<ast> Expr

%%

Program:
    Func_def {printf("PRogram start \n");};

Func_def:
    "def" Header ":" Func_def_dec  Stmt_body "end" {printf("Function Definition\n");};

Func_def_dec:
    Func_def_dec Func_def {printf("New Function definition\n");}
|   Func_def_dec Func_Decl {printf("New FUnction Declaration\n");}
|   Func_def_dec Var_Def {printf("New Var Definition\n");}
|   /*ε*/ {printf("End of function definition\n");};

Stmt_body:
    Stmt {printf("Sinle Statement\n");}
|   Stmt_body Stmt {printf("Next Stmt\n");};

Header:
    Type "id" '('')' {printf("Header Decl (Single)\n");}
|   Type "id" '(' Formal Par ')' {printf("Header Decl (Multiple)\n"); }
|   "id" '('')' {printf("Header Decl (Single-notype)\n"); }
|   "id" '(' Formal Par ')' {printf("Header Decl (Multiple-notype)\n"); };


Par:
    Par ';' Formal {printf("no action\n");}
|   /*e*/ {printf("no action\n");};

Formal:
    "ref" Type "id" Var_Comma {printf("REference\n"); }
|   Type "id" Var_Comma {printf("NonReference\n"); };

Var_Comma:
    /* e*/ {printf("VAriablesEnd\n"); }
|   Var_Comma ',' "id" {printf("VariableNext\n"); };

Type:
    "int" {printf("TypeInt\n"); }
| "char" {printf("TypeChar\n"); }
| "bool" {printf("TypeBool\n"); }
| Type '['']' {printf("TypeArray\n"); }
| "list" '['Type']' {printf("TypeList\n"); };

Func_Decl:
    "decl" Header {printf("Function Declaration\n"); };

Var_Def:
    Type "id" {printf("Variable definition (Single)\n"); }
|   Type "id" Var_Comma {printf("VAriable Definition (Multiple)\n");}; 

Stmt:
    Simple { $$ = $1; }
|   "exit" {printf("Exit\n"); }
|   "return" Expr {printf("REturn\n"); }
|   "if" Expr ":" Stmt_body  "end" { printf("If Statement\n");}
|   "if" Expr ":" Stmt_body  "else" ":" Stmt_body "end" {printf("If Else Statement\n");}
|   "for" Simple_List ";" Expr ";" Simple_List ":" Stmt_body "end" {printf("For Statement\n"); };

Simple:
    "skip"          {printf("Skip\n"); }
|   Atom ":=" Expr  {printf("Let\n"); }
|   Call            {printf("Function Call\n"); };

Simple_List:
    Simple                  { printf("Simple Statement (Single)\n");  }
|   Simple  Simple_Comma    { printf("Simple Statement (Multiple)\n");  };

Simple_Comma:
    /*ε*/  { printf("End Simple parameters\n"); }
|   Simple_Comma "," Simple { printf("Simple parameter\n");  };

Call:
    "id"  "("")" { printf("Function Call (Empty)"); }
|   "id"  "(" Expr Expr_Comma ")" { printf("Function Call (Non-Empty)\n");  };


Expr_Comma:
    /*ε*/ { printf("Parameter end\n");  }
|   Expr_Comma "," Expr { printf("Parameter\n");  };


Atom:
    "id"  { printf("Id\n");  }
|   "string_literal"  { printf("String Literal\n");  }
|   Atom "[" Expr "]"  { printf("Array\n");  }
|   Call {printf("Call\n"); };


Expr:
    Atom {$$=$1;}
|   "int_const" { printf("IntConst\n"); }
|   "string_literal" { printf("IntConst\n"); }
|   "char_const" { printf("IntConst\n"); }
|   "(" Expr ")" { printf("ParExpr\n"); }
|   Expr "+" Expr { printf("Plus\n"); }
|   Expr "-" Expr { printf("Minus\n"); }
|   Expr "*" Expr { printf("Epi\n"); }
|   Expr "/" Expr { printf("Dia\n"); }
|   Expr "mod" Expr { printf("BinOpMod\n"); }
|   Expr "=" Expr { printf("BinOpEq\n"); }
|   Expr "<>" Expr { printf("BinOpNotEq\n"); }
|   Expr "<" Expr { printf("BinOpLessThan\n"); }
|   Expr ">" Expr { printf("BinOpGreaterThan\n"); }
|   Expr "<=" Expr { printf("BinOpLessEqThan\n"); }
|   Expr ">=" Expr { printf("BinOpGreaterEqThan\n"); }
|   "-" Expr %prec UMINUS { printf("UnOp Uminus\n"); }
|   "+" Expr %prec UPLUS { printf("UnOp Uplus\n"); }
|   "true" { printf("True\n");  }
|   "false" { printf("False\n"); }
|   "not" Expr {printf("Not\n");  }
|   Expr "and" Expr { printf("And\n");  }
|   Expr "or" Expr { printf("Or\n");  }
|   "new" Type "[" Expr "]" { printf("New\n"); }
|   "nil" { printf("nil\n"); }
|   "nil?" "(" Expr ")" { printf("nil\n");  }
|   Expr "#" Expr { printf("BinOp\n"); }
|   "head" "(" Expr ")" { printf("UnOp Head\n"); }
|   "tail" "(" Expr ")" { printf("UnOp Tail\n"); };


%%

int main(){
  return 0;
}