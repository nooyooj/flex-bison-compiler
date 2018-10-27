%{
#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"
%}

%union {
	struct ast *a;
	double d;
	struct symbol *s;
	struct symbolList *symbolList;
	struct numList *numList;
	int fn;
	char type_c;
}

%token <d> NUMBER
%token <s> ID
%token PROGRAM VAR ARRAY OF INTEGER REAL BGN END IF THEN ELSE WHILE DO DOTS PRINT
%token <type_c> STD_TYPE

%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*' '/'
%nonassoc '|' UMINUS

%type <a> decl_list declaration stmt_list stmt exp
%type <symbolList> symbolList
%type <numList> numList

%start program
%%

program: PROGRAM ID '(' symbolList ')' ';' decl_list BGN stmt_list END '.'
	{ eval($7); eval($9); treefree($9); treefree($7); printf("parse done.\n"); }
	;

decl_list:	{ $$ = NULL; }
	| declaration ';' decl_list	{ if($3 == NULL) $$ = $1; else $$ = newAst('L', $1, $3); }
	;

declaration: VAR symbolList ':' STD_TYPE	{ $$ = newDeclaration($2, $4); }
	| VAR symbolList ':' ARRAY '[' NUMBER DOTS NUMBER ']' OF STD_TYPE
	{ $$ = newDeclarationArr($2, $6, $8, $11); }
	;

stmt: IF exp THEN '{' stmt_list '}'		{ $$ = newflow('I', $2, $5, NULL); }
	| IF exp THEN '{' stmt_list '}' ELSE '{' stmt_list '}'	{ $$ = newflow('I', $2, $5, $9); }
	| WHILE exp DO '{' stmt_list '}'	{ $$ = newflow('W', $2, $5, NULL); }
	| exp
	;

stmt_list: stmt		{ $$ = $1; }
	| stmt_list ';' stmt { $$ = newAst('L', $1, $3); }
	;

exp: exp CMP exp	{ $$ = newCmp($2, $1, $3); }
	| exp '+' exp	{ $$ = newAst('+', $1, $3); }
	| exp '-' exp	{ $$ = newAst('-', $1, $3); }
	| exp '*' exp	{ $$ = newAst('*', $1, $3); }
	| exp '/' exp 	{ $$ = newAst('/', $1, $3); }
	| '|' exp	{ $$ = newAst('|', $2, NULL); }
	| '(' exp ')'	{ $$ = $2; }
	| '-' exp %prec UMINUS	{ $$ = newAst('M', $2, NULL); }
	| NUMBER	{ $$ = newNum($1); }
	| ID		{ $$ = newRef($1); }
	| ID '[' exp ']'	{ $$ = newRefArr($1, $3); }
	| ID '[' exp ']' '=' exp	{ $$ = newAssignArr($1, $3, $6); }
	| ID '=' exp	{ $$ = newAssign($1, $3); }
	| ID '=' '{' numList '}'	{ $$ = newInitialArr($1, $4); }	
	| PRINT '(' exp ')'	{ $$ = newPrint($3); }
	;

numList: NUMBER	{ $$ = newNumList($1, NULL); }	
	| NUMBER ',' numList	{ $$ = newNumList($1, $3); }
	;

symbolList: ID	{ $$ = newSymbolList($1, NULL); }
	| ID ',' symbolList	{ $$ = newSymbolList($1, $3); }
	;
%%
