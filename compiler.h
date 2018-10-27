void yyerror(char *s);

int yyparse();
int yylex();

struct symbol 
{
	double *arr_head;
	double value;

	char *name;
	char type;

	int arr_len;
	int ini_index;
};

#define NHASH 9997
struct symbol symbolTable[NHASH];

struct symbol *lookUp(char *);

struct symbolList 
{
	struct symbol *symbol;
	struct symbolList *next;
};

struct numList 
{
	double number;

	struct numList *next;
};

struct symbolList *newSymbolList(struct symbol *symbol, struct symbolList *next);
struct numList *newNumList(double number, struct numList *numList);

struct ast 
{
	int nodeType;

	struct ast *left;
	struct ast *right;
};

struct printCall 
{
	int nodeType;

	struct ast *left;
};

struct flow 
{
	int nodeType;

	struct ast *cond;
	struct ast *tl;
	struct ast *el;
};

struct numVal 
{
	int nodeType;

	double number;
};

struct symbolRef 
{
	int nodeType;

	struct symbol *symbol;
};

struct symbolRefArr 
{
	int nodeType;

	struct symbol *symbol;

	struct ast *index;
};

struct symbolAssign 
{
	int nodeType;

	struct symbol *symbol;

	struct ast *v;
};

struct symbolAssignArr 
{
	int nodeType;
	
	struct symbol *symbol;

	struct ast *index;
	struct ast *v;
};

struct symbolInitialArr 
{
	int nodeType;

	struct symbol *symbol;
	struct numList *numList;
};

struct declaration 
{
	int nodeType;

	struct symbolList *symbolList;

	char type;
};

struct declarationArr
{
	int nodeType;
	struct symbolList *symbolList;
	int length;
	int shift;
	char type;
};

struct ast *newAst(int nodeType, struct ast *left, struct ast *right);
struct ast *newCmp(int cmpType, struct ast *left, struct ast *right);
struct ast *newPrint(struct ast *left);
struct ast *newRef(struct symbol *symbol);
struct ast *newAssign(struct symbol *symbol, struct ast *v);
struct ast *newNum(double number);


struct ast *newDeclaration(struct symbolList *symbolList, char type);
struct ast *newDeclarationArr(struct symbolList *symbolList, int begin, int end, char type);
struct ast *newAssignArr(struct symbol *symbol, struct ast *index, struct ast *a);
struct ast *newRefArr(struct symbol *symbol, struct ast *index);
struct ast *newInitialArr(struct symbol *symbol, struct numList *numList);


