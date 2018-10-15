# flex-bison-compiler
Compiler construction using Flex and Bison

## Setup Instructions
Clone this repository. You will need `flex`, `bison`, and `gcc` installed on your machine.

#### For Flex:
`$ flex lexer.l`
<br />
compiled: `lex.yy.c`

#### For Bison:
`$ bison -d compiler.y`
<br />
compiled: `compiler.tab.c`, `compiler.tab.h`
