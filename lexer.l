%option noyywrap

%{
#define T_eof 0
#define T_id 1001 
#define T_print 1002
#define T_let 1003
#define T_for 1004
#define T_do 1005
#define T_begin 1006
#define T_end 1007
#define T_if 1008
#define T_then 1009
#define T_var 1010
#define T_const 1011
void ERROR (const char msg []);
%}

L [a-z]
D [0-9]
W [ \r\t\n]

%%

"let" {return T_let;}
"for" {return T_for;}
"do" {return T_do;}
"begin" {return T_begin;}
"end" {return T_end;}
"if" {return T_if;}
"then" {return T_then;}
"var" {return T_var;}
"print" {return T_print;}
[1-9]{D}*|0 {return T_const;}
"+"|"-"|"*"|"/"|"%"|"=" {return yytext[0];}
{L} {return T_id;} 
\'.*\n { /* nothing */ }
{W}+    { /* nothing */ }
. {printf("error!\n"); exit (1);}

%%

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