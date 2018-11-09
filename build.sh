flex lexer.l
bison -d grammar.y
gcc -o compiler compiler.c grammar.tab.c lex.yy.c
