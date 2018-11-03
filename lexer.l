%option noyywrap
%{
#include "compiler.h"
#include "grammar.tab.h"
%}

%%
".."	{ return DOTS; }

"+"	|
"-" 	|
"*"	|
"/"	|
"="	|
"|"	|
","	|
";"	|
":"	|
"."	|
"["	|
"]"	|
"{"	|
"}"	|
"("	|
")"	{ return yytext[0]; }

">"	{ yylval.fn = 1; return CMP; }
"<"	{ yylval.fn = 2; return CMP; }
"<>"	{ yylval.fn = 3; return CMP; }
"=="	{ yylval.fn = 4; return CMP; }
">="	{ yylval.fn = 5; return CMP; }
"<="	{ yylval.fn = 6; return CMP; }

"integer"	{ yylval.typeC = 'a'; return STD_TYPE; }
"real"		{ yylval.typeC = 'b'; return STD_TYPE; }

"program"	{ return PROGRAM; }
"var"	{ return VAR; }
"array"	{ return ARRAY; }
"of"	{ return OF; }
"begin"	{ return BGN; }
"end"	{ return END; }

"if"	{ return IF; }
"then"	{ return THEN; }
"else"	{ return ELSE; }
"while"	{ return WHILE; }
"do"	{ return DO; }

"print"	{ return PRINT; }

[a-zA-Z][a-zA-Z0-9]*	{ yylval.symbol = lookUp(yytext); return SYMBOL; }

[0-9]+"."[0-9]+	|
[0-9]+	{ yylval.d = atof(yytext); return NUMBER; }

"//".*
[ \t\n]
.	{ yyerror("Mystery character.\n"); }
%%
