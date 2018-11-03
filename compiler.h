void yyerror(char *s);

int yyparse();
int yylex();

struct symbol 
{
	double *arrHead;
	double value;

	char *name;
	char type;

	int arrLength;
	int initialIndex;
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
	struct ast *treeList;
	struct ast *evalList;
};

struct numVal 
{
	int nodeType;

	double number;
};

struct symbolReference
{
	int nodeType;

	struct symbol *symbol;
};

struct symbolReferenceArr 
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
struct ast *newCompare(int compareType, struct ast *left, struct ast *right);
struct ast *newPrint(struct ast *left);
struct ast *newReference(struct symbol *symbol);
struct ast *newAssign(struct symbol *symbol, struct ast *v);
struct ast *newNum(double number);
struct ast *newFlow(int nodeType, struct ast *cond, struct ast *tl, struct ast *tr);

struct ast *newDeclaration(struct symbolList *symbolList, char type);
struct ast *newDeclarationArr(struct symbolList *symbolList, int begin, int end, char type);
struct ast *newAssignArr(struct symbol *symbol, struct ast *index, struct ast *a);
struct ast *newReferenceArr(struct symbol *symbol, struct ast *index);
struct ast *newInitialArr(struct symbol *symbol, struct numList *numList);

double eval(struct ast *);

void treeFree(struct ast *);

