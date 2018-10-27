#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "compiler.h"

static unsigned symbolHash(char *symbol)
{
	unsigned int hash = 0;

	unsigned ch;

	while(ch = *symbol++)
	{
		hash = hash * 9 ^ ch;

		return hash;
	}
}

struct symbol *lookUp(char *symbol)
{
	struct symbol *s = &symbolTable[symbolHash(symbol) % NHASH];

	int symbolCount = NHASH;

	while(--symbolCount >= 0)
	{
		if(s->name && !strcmp(s->name, symbol))
		{
			return s;
		}

		if(!s->name)
		{
			s->name = strdup(symbol);
			s->value = 0;

			return s;
		}

		if(++s >= symbolTable + NHASH)
		{
			s = symbolTable;
		}
	}

	yyerror("symbol table overflow occurred\n");

	abort();
}

struct ast *newAst(int nodeType, struct ast *left, struct ast *right)
{
	struct ast *a = malloc(sizeof(struct ast));

	if(!a)
	{
		yyerror("out of space");

		exit(0);
	}

	a->nodeType = nodeType;
	a->left = left;
	a->right = right;

	return a;
}

struct ast *newNum(double number)
{
	struct numVal *n = malloc(sizeof(struct numVal));

	if(!n)
	{
		yyerror("out of space");

		exit(0);
	}

	n->nodeType = 'K';
	n->number = number;

	return (struct ast *)n;
}

struct ast *newCmp(int cmpType, struct ast *left, struct ast *right)
{
	struct ast *a = malloc(sizeof(struct ast));

	if(!a)
	{
		yyerror("out of space");

		exit(0);
	}

	a->nodeType = '0' + cmpType;
	a->left = left;
	a->right = right;

	return a;
}

struct ast *newPrint(struct ast *left)
{
	struct printCall *p = malloc(sizeof(struct printCall));

	if(!p)
	{
		yyerror("out of space");

		exit(0);
	}

	p->nodeType = 'P';
	p->left = left;

	return (struct ast *)p;
}

struct ast *newRef(struct symbol *symbol)
{
	struct symbolRef *sr = malloc(sizeof(struct symbolRef));

	if(!sr)
	{
		yyerror("out of space");

		exit(0);
	}

	sr->nodeType = 'N';
	sr->symbol = symbol;

	return (struct ast *)sr;
}

struct ast *newRefArr(struct symbol *symbol, struct ast *index)
{
	struct symbolRefArr *sra = malloc(sizeof(struct symbolRefArr));

	if(!sra)
	{
		yyerror("out of space");

		exit(0);
	}

	sra->nodeType = 'U';
	sra->symbol = symbol;
	sra->index = index;

	return (struct ast *)sra;
}

struct ast *newAssign(struct symbol *symbol, struct ast *v)
{
	struct symbolAssign *sa = malloc(sizeof(struct symbolAssign));

	if(!sa)
	{
		yyerror("out of space");

		exit(0);
	}

	sa->nodeType = '=';
	sa->symbol = symbol;
	sa->v = v;

	return (struct ast *)sa;
}

struct ast *newAssignArr(struct symbol *symbol, struct ast *index, struct ast *v)
{
	struct symbolAssignArr *saa = malloc(sizeof(struct symbolAssignArr));

	if(!saa)
	{
		yyerror("out of space");
		
		exit(0);
	}

	saa->nodeType = 'V';
	saa->symbol = symbol;
	saa->index = index;
	saa->v = v;

	return (struct ast *)saa;
}

struct ast *newDeclaration(struct symbolList *symbolList, char type)
{
	struct declaration *d = malloc(sizeof(struct declaration));

	if(!d)
	{
		yyerror("out of space");
		
		exit(0);
	}

	d->nodeType ='X';
	d->symbolList = symbolList;
	d->type = type;

	return (struct ast *)d;
}

struct ast *newDeclarationArr(struct symbolList *symbolList, int begin, int end, char type)
{
	struct declarationArr *da = malloc(sizeof(struct declarationArr));

	if(!da)
	{
		yyerror("out of space");
		
		exit(0);
	}

	da->nodeType = 'Y';
	da->symbolList = symbolList;
	da->length = end - begin + 2;

	if(da->length < 1)
	{
		yyerror("too small size for array");
		
		exit(0);
	}

	da->shift = begin;
	da->type = type;

	return (struct ast *)da;
}

struct symbolList *newSymbolList(struct symbol *symbol, struct symbolList *next)
{
	struct symbolList *sl = malloc(sizeof(struct symbolList));

    	if(!sl)
    	{
        	yyerror("out of space");

        	exit(0);
 	   }
 
    	sl->symbol = symbol;
    	sl->next = next;
     
	return sl;
}

struct numList *newNumList(double number, struct numList *next)
{
    struct numList *nl = malloc(sizeof(struct numList));
    
    if(!nl)
    {
        yyerror("out of space");
        
        exit(0);
    }   
    
    nl->number = number;
    nl->next = next;

    return nl;
}

struct ast *newInitialArr(struct symbol *symbol, struct numList *numList)
{
    struct symbolInitialArr *sia = malloc(sizeof(struct symbolInitialArr));
    
    if(!sia)
    {
        yyerror("out of space");

        exit(0);
    }   
     
    sia->nodeType = 'T';
    sia->symbol = symbol;
    sia->numList = numList;
     
    return (struct ast *)sia;
}

void yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
}

int main(int argc, char**argv)
{
	extern FILE *yyin;
	++argv; --argc;
	yyin = fopen(argv[0], "r");
	return yyparse();
}
