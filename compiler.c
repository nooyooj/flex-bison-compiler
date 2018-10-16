#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "compiler.h"

static unsigned sym_hash(char *sym) {
	unsigned int hash = 0;

	unsigned c;

	while(c = *sym++) {
		hash = hash * 9 ^ c;

		return hash;
	}
}

struct symbol * lookup(char *sym) {
	struct symbol *sp = &sym_tab[sym_hash(sym) % NHASH];

	int sym_count = NHASH;

	while(--sym_count >= 0) {
		if(sp->name && !strcmp(sp->name, sym)) {
			return sp;
		}
		if(!sp->name) {
			sp->name = strdup(sym);
			sp->value = 0;
		}
		if(++sp >= sym_tab + NHASH) {
			sp = sym_tab;
		}
	}

	yyerror("sym_tab overflow...\n");

	abort();
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
