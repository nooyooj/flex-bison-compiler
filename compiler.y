%{
	#include <stdio.h>
	#include <stdlib.h>
}

%union {
	struct ast *a;
	struct symbol *s;
	struct symlist *sl;
	struct numlist *nl;
	int fn;
	double d;
	char type_c;
}

%token <d> NUMBER
%token <s> ID
%token PROGRAM VAR ARRAY OF INTEGER REAL BGN END IF THEN ELSE WHILE DO DOTS PRINT
%token <type_c> STD_TYPE

%nonassoc <fn> CMP
%nonassoc '|' UMINUS

%right '='

%left '+' '-'
%left '*' '/'

%type <a> decl_list decl stmt_list stmt exp
%type <sl> id_list
%type <nl> num_list

%start program
%%

program: PROGRAM ID '(' id_list ')' ';' decl_list BGN stmt_list END '.'
	{ eval($7); eval($9); treefree($9); treefree($7); printf("parse done.\n"); }
	;

