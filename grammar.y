%{
#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"
%}

%union {
	struct ast *a;
	double d;
	struct symbol *symbol;
	struct symbolList *symbolList;
	struct numList *numList;
	int fn;
	char typeC;
}

%token <d> NUMBER
%token <symbol> SYMBOL
%token PROGRAM VAR ARRAY OF INTEGER REAL BGN END IF THEN ELSE WHILE DO DOTS PRINT
%token <typeC> STD_TYPE

%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*' '/'
%nonassoc '|' UMINUS

%type <a> declarationList declaration statementList statement expression
%type <symbolList> symbolList
%type <numList> numList

%start program
%%

program: PROGRAM SYMBOL '(' symbolList ')' ';' declarationList BGN statementList END '.'
	{ eval($7); eval($9); treeFree($9); treeFree($7); printf("parse done.\n"); }
	;

declarationList:	{ $$ = NULL; }
	| declaration ';' declarationList	{ if($3 == NULL) $$ = $1; else $$ = newAst('L', $1, $3); }
	;

declaration: VAR symbolList ':' STD_TYPE	{ $$ = newDeclaration($2, $4); }
	| VAR symbolList ':' ARRAY '[' NUMBER DOTS NUMBER ']' OF STD_TYPE
	{ $$ = newDeclarationArr($2, $6, $8, $11); }
	;

statement: IF expression THEN '{' statementList '}'		{ $$ = newFlow('I', $2, $5, NULL); }
	| IF expression THEN '{' statementList '}' ELSE '{' statementList '}'	{ $$ = newFlow('I', $2, $5, $9); }
	| WHILE expression DO '{' statementList '}'	{ $$ = newFlow('W', $2, $5, NULL); }
	| expression
	;

statementList: statement		{ $$ = $1; }
	| statementList ';' statement { $$ = newAst('L', $1, $3); }
	;

expression: expression CMP expression	{ $$ = newCompare($2, $1, $3); }
	| expression '+' expression	{ $$ = newAst('+', $1, $3); }
	| expression '-' expression	{ $$ = newAst('-', $1, $3); }
	| expression '*' expression	{ $$ = newAst('*', $1, $3); }
	| expression '/' expression 	{ $$ = newAst('/', $1, $3); }
	| '|' expression	{ $$ = newAst('|', $2, NULL); }
	| '(' expression ')'	{ $$ = $2; }
	| '-' expression %prec UMINUS	{ $$ = newAst('M', $2, NULL); }
	| NUMBER	{ $$ = newNum($1); }
	| SYMBOL		{ $$ = newReference($1); }
	| SYMBOL '[' expression ']'	{ $$ = newReferenceArr($1, $3); }
	| SYMBOL '[' expression ']' '=' expression	{ $$ = newAssignArr($1, $3, $6); }
	| SYMBOL '=' expression	{ $$ = newAssign($1, $3); }
	| SYMBOL '=' '{' numList '}'	{ $$ = newInitialArr($1, $4); }	
	| PRINT '(' expression ')'	{ $$ = newPrint($3); }
	;

numList: NUMBER	{ $$ = newNumList($1, NULL); }	
	| NUMBER ',' numList	{ $$ = newNumList($1, $3); }
	;

symbolList: SYMBOL	{ $$ = newSymbolList($1, NULL); }
	| SYMBOL ',' symbolList	{ $$ = newSymbolList($1, $3); }
	;
%%
