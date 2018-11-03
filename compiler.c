#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "compiler.h"

static unsigned symbolHash(char *symbol) {
	unsigned int hash = 0;

	unsigned ch;

	while(ch = *symbol++) {
		hash = hash * 9 ^ ch;

		return hash;
	}
}

struct symbol *lookUp(char *symbol) {
	struct symbol *s = &symbolTable[symbolHash(symbol) % NHASH];

	int symbolCount = NHASH;

	while(--symbolCount >= 0) {
		if(s->name && !strcmp(s->name, symbol)) {
			return s;
		}
		if(!s->name) {
			s->name = strdup(symbol);
			s->value = 0;

			return s;
		}
		if(++s >= symbolTable + NHASH) {
			s = symbolTable;
		}
	}

	yyerror("symbol table overflow occurred\n");

	abort();
}

struct ast *newAst(int nodeType, struct ast *left, struct ast *right) {
	struct ast *a = malloc(sizeof(struct ast));

	if(!a) {
		yyerror("out of space");

		exit(0);
	}

	a->nodeType = nodeType;
	a->left = left;
	a->right = right;

	return a;
}

struct ast *newNum(double number) {
	struct numVal *n = malloc(sizeof(struct numVal));

	if(!n) {
		yyerror("out of space");

		exit(0);
	}

	n->nodeType = 'K';
	n->number = number;

	return (struct ast *)n;
}

struct ast *newCompare(int compareType, struct ast *left, struct ast *right) {
	struct ast *a = malloc(sizeof(struct ast));

	if(!a) {
		yyerror("out of space");

		exit(0);
	}

	a->nodeType = '0' + compareType;
	a->left = left;
	a->right = right;

	return a;
}

struct ast *newPrint(struct ast *left) {
	struct printCall *p = malloc(sizeof(struct printCall));

	if(!p) {
		yyerror("out of space");

		exit(0);
	}

	p->nodeType = 'P';
	p->left = left;

	return (struct ast *)p;
}

struct ast *newReference(struct symbol *symbol) {
	struct symbolReference *sr = malloc(sizeof(struct symbolReference));

	if(!sr) {
		yyerror("out of space");

		exit(0);
	}

	sr->nodeType = 'N';
	sr->symbol = symbol;

	return (struct ast *)sr;
}

struct ast *newReferenceArr(struct symbol *symbol, struct ast *index) {
	struct symbolReferenceArr *sra = malloc(sizeof(struct symbolReferenceArr));

	if(!sra) {
		yyerror("out of space");

		exit(0);
	}

	sra->nodeType = 'U';
	sra->symbol = symbol;
	sra->index = index;

	return (struct ast *)sra;
}

struct ast *newAssign(struct symbol *symbol, struct ast *v) {
	struct symbolAssign *sa = malloc(sizeof(struct symbolAssign));

	if(!sa) {
		yyerror("out of space");

		exit(0);
	}

	sa->nodeType = '=';
	sa->symbol = symbol;
	sa->v = v;

	return (struct ast *)sa;
}

struct ast *newAssignArr(struct symbol *symbol, struct ast *index, struct ast *v) {
	struct symbolAssignArr *saa = malloc(sizeof(struct symbolAssignArr));

	if(!saa) {
		yyerror("out of space");
		
		exit(0);
	}

	saa->nodeType = 'V';
	saa->symbol = symbol;
	saa->index = index;
	saa->v = v;

	return (struct ast *)saa;
}

struct ast *newDeclaration(struct symbolList *symbolList, char type) {
	struct declaration *d = malloc(sizeof(struct declaration));

	if(!d) {
		yyerror("out of space");
		
		exit(0);
	}

	d->nodeType ='X';
	d->symbolList = symbolList;
	d->type = type;

	return (struct ast *)d;
}

struct ast *newDeclarationArr(struct symbolList *symbolList, int begin, int end, char type) {
	struct declarationArr *da = malloc(sizeof(struct declarationArr));

	if(!da) {
		yyerror("out of space");
		
		exit(0);
	}

	da->nodeType = 'Y';
	da->symbolList = symbolList;
	da->length = end - begin + 2;

	if(da->length < 1) {
		yyerror("too small size for array");
		
		exit(0);
	}

	da->shift = begin;
	da->type = type;

	return (struct ast *)da;
}

struct symbolList *newSymbolList(struct symbol *symbol, struct symbolList *next) {
	struct symbolList *sl = malloc(sizeof(struct symbolList));

    	if(!sl) {
        	yyerror("out of space");

        	exit(0);
 	}
 
    	sl->symbol = symbol;
    	sl->next = next;
     
	return sl;
}

struct numList *newNumList(double number, struct numList *next) {
    struct numList *nl = malloc(sizeof(struct numList));
    
    if(!nl) {
        yyerror("out of space");
        
        exit(0);
    }   
    
    nl->number = number;
    nl->next = next;

    return nl;
}

struct ast *newInitialArr(struct symbol *symbol, struct numList *numList) {
    struct symbolInitialArr *sia = malloc(sizeof(struct symbolInitialArr));
    
    if(!sia) {
        yyerror("out of space");

        exit(0);
    }   
     
    sia->nodeType = 'T';
    sia->symbol = symbol;
    sia->numList = numList;
     
    return (struct ast *)sia;
}

struct ast *newFlow(int nodeType, struct ast *cond, struct ast *treeList, struct ast *evalList) {
	struct flow *f = malloc(sizeof(struct flow));

	if(!f) {
		yyerror("out of space");

		exit(0);
	}

	f->nodeType = nodeType;
	f->cond = cond;
	f->treeList = treeList;
	f->evalList = evalList;

	return (struct ast *)f;
}

void treeFree(struct ast *a) {
	switch(a->nodeType) {
		case '+':

		case '-':

		case '*':

		case '/':

		case '1': 

		case '2': 
		
		case '3': 

		case '4': 

		case '5': 
	
		case '6':

		case 'L':
			treeFree(a->right);

		case '|':

		case 'M': 

		case 'P':
			treeFree(a->left);

		case 'K': 

		case 'N': 

		case 'U': 

		case 'V': 

		case 'T':
			break;

		case '=':
			free(((struct symbolAssign *)a)->v);

			break;

		case 'I': case 'W':
			free(((struct flow *)a)->cond);

			if(((struct flow *)a)->treeList) {
				treeFree(((struct flow *)a)->treeList);
			}
			if(((struct flow *)a)->evalList) {
				treeFree(((struct flow *)a)->evalList);
			}

			break;

		case 'X':
			free(((struct declaration *)a)->symbolList);

			break;

		case 'Y':
			free(((struct declarationArr *)a)->symbolList);

			break;

		default: printf("internal error: free bad node %c\n", a->nodeType);
	}

	free(a);
}

static double callPrint(struct printCall *);
static double callDeclaration(struct declaration *);
static double callDeclarationArr(struct declarationArr *);
static double callInitialArr(struct symbolInitialArr *ia);
static double callAssignArr(struct symbolAssignArr *);
static double callReferenceArr(struct symbolReferenceArr *);

double eval(struct ast *a) {
	double v;

	if(!a) {
		yyerror("internal error, null eval");

		return 0.0;
	}

	switch(a->nodeType) {
	
		case 'K': 
			v = ((struct numVal *)a)->number; 

			break;

		case 'N': 
			if((((struct symbolReference *)a)->symbol)->type != 'a' && (((struct symbolReference *)a)->symbol)->type != 'b') { 
				printf("using undeclared ID: %s\n", (((struct symbolReference *)a)->symbol)->name); 
			}

			v = (((struct symbolReference *)a)->symbol)->value; 

			break;

		case '=': 
			if((((struct symbolAssign *)a)->symbol)->type != 'a' && (((struct symbolAssign *)a)->symbol)->type != 'b') { 
				printf("using undeclared ID: %s\n", (((struct symbolAssign *)a)->symbol)->name); 
			} 

			v = ((struct symbolAssign *)a)->symbol->value = eval(((struct symbolAssign *)a)->v); 

			break;

		case '+': 
			v = eval(a->left) + eval(a->right); 
			
			break;

		case '-': 
			v = eval(a->left) - eval(a->right); 
			
			break;

		case '*': 
			v = eval(a->left) * eval(a->right); 

			break;

		case '/': 
			v = eval(a->left) / eval(a->right); 

			break;

		case '|': 
			v = fabs(eval(a->left)); 
		
			break;

		case 'M': 
			v = -eval(a->left); 

			break;

		case '1': 
			v = (eval(a->left) > eval(a->right))? 1 : 0; 
			
			break;

		case '2': 
			v = (eval(a->left) < eval(a->right))? 1 : 0; 
			
			break;
		
		case '3': 
			v = (eval(a->left) != eval(a->right))? 1 : 0; 
			
			break;

		case '4': 
			v = (eval(a->left) == eval(a->right))? 1 : 0; 
			
			break;

		case '5': 
			v = (eval(a->left) >= eval(a->right))? 1 : 0; 
			
			break;

		case '6': 
			v = (eval(a->left) <= eval(a->right))? 1 : 0; 
			
			break;

		case 'I':
			if(eval(((struct flow *)a)->cond) != 0) {
				if(((struct flow *)a)->treeList) {
					v = eval(((struct flow *)a)->treeList);
				} 
				else {
					v = 0.0;
				}
			} 
			else {
				if(((struct flow *)a)->evalList) {
					v = eval(((struct flow *)a)->evalList);
				} 
				else {
					v = 0.0;
				}
			}

			break;

		case 'W':
			v = 0.0;

			if(((struct flow *)a)->treeList) {
				while(eval(((struct flow *)a)->cond) != 0)
					v = eval(((struct flow *)a)->treeList);
			}

			break;

		case 'L': 
			eval(a->left); v = eval(a->right); 

			break;

		case 'P': 
			v = callPrint((struct printCall *)a); 

			break;

		case 'X': 
			v = callDeclaration((struct declaration *)a); 

			break;

		case 'Y': 
			v = callDeclarationArr((struct declarationArr *)a); 

			break;

		case 'T': 
			if((((struct symbolInitialArr *)a)->symbol)->type != 'a' && (((struct symbolInitialArr *)a)->symbol)->type != 'b') {
				printf("using undeclared ID: %s\n", (((struct symbolInitialArr *)a)->symbol)->name);
			}

			v = callInitialArr((struct symbolInitialArr *)a); 
		
			break;
		
		case 'U': 
			if((((struct symbolReference *)a)->symbol)->type != 'a' && (((struct symbolReference *)a)->symbol)->type != 'b') { 
				printf("using undeclared ID: %s\n", (((struct symbolReference *)a)->symbol)->name); 
			}

			v = callReferenceArr((struct symbolReferenceArr *)a); 
			
			break;

		case 'V': 
			if((((struct symbolAssignArr *)a)->symbol)->type != 'a' && (((struct symbolAssignArr *)a)->symbol)->type != 'b') { 
				printf("using undeclared ID: %s\n", (((struct symbolAssignArr *)a)->symbol)->name); 
			}
			
			v = callAssignArr((struct symbolAssignArr *)a); 
			
			break;

		default: 
			printf("internal error: bad node %c\n", a->nodeType);
		}

	return v;
}

static double callDeclaration(struct declaration *d) {
	if(d->type == 'a') {
		while(d->symbolList) {
			((d->symbolList)->symbol)->value = 0;
			((d->symbolList)->symbol)->type = 'a';
			((d->symbolList)->symbol)->arrLength = 0;
			((d->symbolList)->symbol)->initialIndex = 0;
			((d->symbolList)->symbol)->arrHead = NULL;

			d->symbolList = (d->symbolList)->next;
		}
	}
	else if(d->type == 'b') {
		while(d->symbolList) {
			((d->symbolList)->symbol)->value = 0.0;
			((d->symbolList)->symbol)->type = 'b';
			((d->symbolList)->symbol)->arrLength = 0;
			((d->symbolList)->symbol)->initialIndex = 0;
			((d->symbolList)->symbol)->arrHead = NULL;

			d->symbolList = (d->symbolList)->next;
		}
	}
	else {
		printf("bad declaration type: %c\n", d->type);	

		return 1;
	}

	return 0;
}

static double callDeclarationArr(struct declarationArr *da) {
	if(da->type == 'a') {
		while(da->symbolList) {
			((da->symbolList)->symbol)->value = -1;
			((da->symbolList)->symbol)->type = 'a';
			((da->symbolList)->symbol)->arrLength = da->length;
			((da->symbolList)->symbol)->initialIndex = da->shift;
			((da->symbolList)->symbol)->arrHead = (double *)malloc(sizeof(double) * da->length);

			if(!(((da->symbolList)->symbol)->arrHead)) {
				yyerror("out of space for array");

				exit(0);
			}

			da->symbolList = (da->symbolList)->next;
		}
	}
	else if(da->type = 'b') {
		while(da->symbolList) {
			((da->symbolList)->symbol)->value = -1.0;
			((da->symbolList)->symbol)->type = 'b';
			((da->symbolList)->symbol)->arrLength = da->length;
			((da->symbolList)->symbol)->initialIndex = da->shift;
			((da->symbolList)->symbol)->arrHead = (double *)malloc(sizeof(double) * da->length);

			if(!(((da->symbolList)->symbol)->arrHead)) {
				yyerror("out of space for array");

				exit(0);
			}

			da->symbolList = (da->symbolList)->next;
		}
	}
	else {
		printf("bad declaration type %c\n", da->type);

		return 1;
	}

	return 0;
}

static double callPrint(struct printCall *f) {
	int i;

	if((f->left)->nodeType == 'N') {
		printf("%s", (((struct symbolReference *)(f->left))->symbol)->name);
	}

	if((f->left)->nodeType == 'N' && (((struct symbolReference *)(f->left))->symbol)->arrLength > 0) {
		printf(" = {");

		for(i = 0; i < (((struct symbolReference *)(f->left))->symbol)->arrLength -1; i++) {
			printf("%4.4g, ", *((((struct symbolReference *)(f->left))->symbol)->arrHead + i));
		}

		printf("%4.4g", *((((struct symbolReference *)(f->left))->symbol)->arrHead + (((struct symbolReference *)(f->left))->symbol)->arrLength - 1));

		printf("}\n");

		return 0;
	}

	double v = eval(f->left);

	printf(" = %4.4g\n", v);
	
	return v;
}

static double callInitialArr(struct symbolInitialArr *symbolInitialArr) {	
	int i;

	for(i = 0; i < ((symbolInitialArr->symbol)->arrLength) && (symbolInitialArr->numList) != NULL; i++) {
		*((symbolInitialArr->symbol)->arrHead + i) = (symbolInitialArr->numList)->number;

		symbolInitialArr->numList = (symbolInitialArr->numList)->next;
	}

	return 0;
}

static double callReferenceArr(struct symbolReferenceArr *symbolReferenceArr)
{
	int arrIndex = (int)eval(symbolReferenceArr->index);

	if((symbolReferenceArr->symbol)->arrLength == 0) {
		yyerror("wrong reference");

		exit(0);
	}

	return *(((symbolReferenceArr->symbol)->arrHead) + arrIndex - ((symbolReferenceArr->symbol)->initialIndex));
}

static double callAssignArr(struct symbolAssignArr *symbolAssignArr)
{
	int arrIndex = (int)eval(symbolAssignArr->index);

	double d = eval(symbolAssignArr->v);

	*(((symbolAssignArr->symbol)->arrHead) + arrIndex - ((symbolAssignArr->symbol)->initialIndex)) = d;

	return d;
}

void yyerror(char *s) {
	fprintf(stderr, "error: %s\n", s);
}

int main(int argc, char**argv) {
	extern FILE *yyin;

	++argv; 
	--argc;

	yyin = fopen(argv[0], "r");

	return yyparse();
}
