%option noyywrap
%option yylineno
%x COMMENT

%{
#include <cstdio>
#include <cstdlib>
#include "lexer.hpp"

%}

%{
#define T_eof 0
#define T_id 1001 
#define T_and 1002
#define T_bool 1003
#define T_char 1004
#define T_decl 1005
#define T_def 1006
#define T_else 1007
#define T_elseif 1008
#define T_end 1009
#define T_exit 1010
#define T_false 1011
#define T_for 1012
#define T_head 1013
#define T_if 1014
#define T_int 1015
#define T_list 1016
#define T_mod 1017
#define T_new 1018
#define T_nil 1019
#define T_nil2 1020
#define T_not 1021
#define T_or 1022
#define T_ref 1023
#define T_return 1024
#define T_skip 1025
#define T_tail 1026
#define T_true 1027
#define T_const 1028
#define T_singlechar 1029
#define T_string 1030
#define T_operator 1031
#define T_seperator 1032


void ERROR (const char msg []);
%}

    int comment_counter = 0;
    int linecount=1;


L [a-zA-Z]
D [0-9]
W [ \r\t]
E [ntr0'"\\]|[x][0-9A-F][0-9A-F]
O [\+\-\*\/\#\=\<\>]|"<>"|"<="|">="
S [\(\)\[\]\,\;\:]|":="
P [\x20-\x7e]

%%
"and" { return T_and;}
"bool" {return T_bool;}
"char" {return T_char;}
"decl" {return T_decl;}
"def" {return T_def;}
"else" {return T_else;}
"elseif" {return T_elseif;}
"end" {return T_end;}
"exit" {return T_exit;}
"false" {return T_false;}
"for" {return T_for;}
"head" {return T_head;}
"if" {return T_if;}
"int" {return T_int;}
"list" {return T_list;}
"mod" { return T_mod;}
"new" {return T_new;}
"nil" {return T_nil;}
"nil?" {return T_nil2;}
"not" {return T_not;}
"or" {return T_or;}
"ref" {return T_ref;}
"return" {return T_return;}
"skip" {return T_skip;}
"tail" {return T_tail;}
"true" {return T_true;}


{D}+ {return T_const;}

{O}  {
    
    return T_operator;}

{S}  {
    
    return T_seperator;}

{L}({L}|{D}|"_"|"?")* {
    return T_id;} 

"'"([\x20-\x26\x28-\x7e]|\\{E})"'" {return T_singlechar;}

\"([\x20-\x21\x23-\x7e]|\\\")*\" {
    return T_string;}

%[^\n]*\n			{ /* nothing */ linecount++; }

\<\* { BEGIN(COMMENT); }
<COMMENT>\<\* {++comment_counter;}
<COMMENT>\*\> {if (comment_counter > 0) --comment_counter;
                else BEGIN(INITIAL);}

<COMMENT>[^\n]          {/* do nothing */}
<COMMENT>\n             {/* change line | do nothing */ linecount++; }
{W}         { /* nothing */ }
\n			{ /* nothing */ linecount++;}

. {yyerror("Error at line ", linecount); exit (1);}



%%

void yyerror(const char *msg, int line) {
  fprintf(stderr, "%s %d\n", msg, line);
  
  exit(1);
}

int main ()
{
    int token;
    do 
    {
        token = yylex();
        printf("token=%d, lexeme=\"%s\"\n", token, yytext);
    } 
    while (token != T_eof);
return 0;
}