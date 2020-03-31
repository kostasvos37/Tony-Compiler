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
  char op[3];
  char sep[2];
  string name;
  string str; 
}

%token<name> T_id 
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
%token<num> T_const 
%token<c> T_singlechar
%token<str> T_string
%token<op> T_operator
%token<sep> T_seperator


%left<op> 'or' 
%left<op> 'and'
%nonassoc<op> 'not'

%nonassoc<op> '=' '<>' '<=' '>=' '<' '>'

%right<op> '#'

%left<op> '+' '-'
%left<op> '*' '/' 'mod'

%left<op> UMINUS UPLUS

%%

Program :
    Func-def;

Func-def :
    "def" Header ":" A  B "end"

A:
    A Func-def
|   A Func-decl
|   A Var-def
|   /*ε*/;

B:
    Stmt
|   B Stmt;

Header:
    Type id '(' Par ')'
|   id '('')'
|   id '(' Par ')';

Par:
    Type Var-Comma
|   "ref" Type Var-Comma
|   Par ';' Par;

Var-Comma :
    id 
|   Var-Comma , id;

Type:
    "int" | "char" | "bool" | Type '['']' | "list" '['Type']';


%%

int main(){
  return 0;
}