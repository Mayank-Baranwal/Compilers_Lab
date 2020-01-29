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
	FILE * fp=fopen("Lexemes.txt","w");
	fp=fopen("Intermediate.txt","w");
	fclose(fp);
	stmt_list();
	if(match(EOI))
		write_in_file("Lexemes.txt","<END_OF_INPUT> ");
	else
    	fprintf( stderr,"%d: Grammar mismatch\n", yylineno );
	convert();
}
