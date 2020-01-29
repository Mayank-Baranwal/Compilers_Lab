#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <ctype.h>
#include "lex.c"
#include "name.c"
#include "code_gen.c"
#include "inter_to_asm.c"

// #include "basic_parser.c"
// #include "improved_parser.c"

void main ()
{
	// FILE * fp=fopen("Lexemes.txt","w");
	// fclose(fp);
	// stmt ();
	// match(EOI);

	convert();
}
