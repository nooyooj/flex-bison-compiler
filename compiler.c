#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "compiler.h"

static unsigned symbol_hash(char *symbol) {
	unsigned int hash = 0;

	unsigned c;

	while(c = *symbol++) {
		hash = hash * 9 ^ c;

		return hash;
	}
}

struct symbol * lookup(char *symbol) {
	struct symbol *sp = &symbol_tab[symbol_hash(symbol) % NHASH];

	int symbol_count = NHASH;

	while(--symbol_count >= 0) {
		if(sp->name && !strcmp(sp->name, symbol)) {
			return sp;
		}
		if(!sp->name) {
			sp->name = strdup(symbol);
			sp->value = 0;
		}
		if(++sp >= symbol_tab + NHASH) {
			sp = symbol_tab;
		}
	}

	yyerror("symbol_tab overflow...\n");

	abort();
}

struct ast *new_ast(int node_type, struct ast *left, struct ast *right) {
	struct ast *a = malloc(sizeof(struct ast));

	if(!a) {
		yyerror("out of space");

		exit(0);
	}
		
	a->node_type = node_type;
	a->left = left;
	a->right = right;
}

struct ast *new_num(double d) {
	struct num_val *a = malloc(sizeof(struct num_val));

	if(!a) {
		yyerror("out of space");

		exit(0);
	}

	a->node_type = 'K';
	a->number = d;

	return (struct ast *)a;
}

void yy_error(char *s) {
	fprintf(stderr, "error: %s\n", s);
}

int main(int argc, char** argv) {
	extern FILE *yy_in;
	
	++argv;
	--argc;
	
	yyin = fopen(argv[0], "r");

	return yy_parse();
}
