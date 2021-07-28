%option noyywrap

%{
#include <cstdio>
#include <cstdlib>
#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"
%}

L [a-z]
D [0-9]
W [ \t\r\n]

%%

"for"   { return T_for; }
"print" { return T_print; }
"let"   { return T_let; }
"if"    { return T_if; }
"then"  { return T_then; }
"else"  { return T_else; }
"do"    { return T_do; }
"begin" { return T_begin; }
"end"   { return T_end; }
"var"   { return T_var; }
"int"   { return T_int; }
"bool"  { return T_bool; }

{L}     { yylval.var = yytext[0]; return T_id; }
{D}+    { yylval.num = atoi(yytext); return T_const; }

[()+\-/%*=<>:]   { yylval.op = yytext[0]; return yytext[0]; }

{W}+    { /* nothing */ }
\'.*\n  { /* nothing' */ }

.       { yyerror("lexical error"); }

%%

void yyerror(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(1);
}
