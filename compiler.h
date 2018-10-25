void yy_error(char *s);
int yy_parse();
int yylex();

struct symbol {
	char *name;
	char type;

	int arr_len;
	int initial_index;

	double value;
	double *arr_head;
};

#define NHASH 9997
struct symbol symbol_tab[NHASH];
struct symbol *lookup(char*);

struct symbol_list {
	struct symbol *symbol;
	struct symbol_list *next;
};

struct num_list {
	double n;

	struct num_list *next;
}

struct symbol_list *new_symbol_list(struct symbol *symbol, struct symbol_list *next);
struct num_list *new_num_list(double n, struct num_list *num_list);

struct ast {
	struct ast *left;
	struct ast *right;

	int node_type;
};

struct printcall {
	struct ast *left;

	int node_type;
};

struct flow {
	struct ast *cond;
	struct ast *t_left;
	struct ast *e_left;

	int node_type;
};

struct num_val {
	int node_type;

	double number;
};

struct symbol_ref {
	int node_type;
	struct symbol *symbol;
};
