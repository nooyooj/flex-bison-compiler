# flex-bison-compiler
Compiler construction using Flex and Bison

## Setup Instructions
Clone this repository. You will need `flex`, `bison`, and `gcc` installed on your machine.

#### To change sh script permission:
`$ chmod +x build.sh clear.sh`

#### To build:
`$ ./build.sh`
<br>
*compiled*: `compiler`, `grammar.tab.c`, `grammar.tab.h`, `lexer.yy.c`

#### To run:
`$ ./compiler FILE_NAME.txt`

#### To clear:
`$ ./clear.sh`
