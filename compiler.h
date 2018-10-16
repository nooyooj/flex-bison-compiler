void yy_error(char *s);
int yy_parse();

struct symbol {
	char *name;
	char type;

	int arr_len;
	int initial_index;

	double value;
	double *arr_head;
};

#define NHASH 9997
struct symbol sym_tab[NHASH];
struct symbol *lookup(char*);

struct sym_list {
	struct symbol *sym;
	struct sym_list *next;
};

struct num_list {
	struct num_list *next;
}

struct sym_list *new_sym_list(struct symbol *sym, struct symlist *next);
struct num_list *new_num_list(double n, struct num_list *nl);

struct ast {
	struct ast *left;
	struct ast *right;

	int node_type;
};

struct printcall {
	struct ast *left;

	int node_type;
}
