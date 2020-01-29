// #include "code_gen.h"

extern char *newname( void       );
extern void freename( char *name );

void stmt_list();
void stmt();
void opt_stmts();
char * expr();
char    *factor     ( void );
char    *term       ( void );
char    *expression ( void );

void write_in_file (char * filename, char * text)
{
	FILE *fp = fopen(filename, "a+");
	fprintf(fp, "%s",text);
	fclose(fp);
}

char *tp;
int done = 0;

void stmt_list ()
{
    if (!done){
		done = 1;
		tp = (char*)malloc (100*sizeof(char));
	}

    char * tempvar;
    while (1)
    {
        stmt();
        if(match(END) || match(EOI))
        {
            break;
        }
        if( match( SEMI ) )
            advance();
        else
            fprintf( stderr,"%d: Inserting missing semicolon\n", yylineno );
    }
}

void stmt ()
{
    char * tempvar;
    if(match(NUM_OR_ID))
    {
    	write_in_file("Lexemes.txt","<ID> ");
        int yyleng_temp=yyleng;
        char yytext_temp[50];
        strcpy(yytext_temp,yytext);
        advance();
        if(!match(COLET))
            fprintf( stderr,"%d: Inserting missing colon equal to\n", yylineno );
        else
            advance();

        tempvar=expr();
        sprintf(tp, "%0.*s = %s\n\09", yyleng_temp, yytext_temp, tempvar);
        write_in_file("Intermediate.txt", tp);
        freename(tempvar);
        return;
    }   
    if (match(IF))
    {
        sprintf(tp, "if(\n");
        write_in_file("Intermediate.txt", tp);
        advance();
        tempvar=expr();
        sprintf(tp, "%s\n)\n", tempvar);
        write_in_file("Intermediate.txt", tp);
        if(!match(THEN))
            fprintf( stderr,"%d: Inserting missing then\n", yylineno );
        else
            advance();
        
        sprintf(tp, "then{\n");
        write_in_file("Intermediate.txt", tp);
        stmt();
       	sprintf(tp, "}\n");
        write_in_file("Intermediate.txt", tp);
        freename(tempvar);
        return;
    }
    if (match(WHILE))
    {
        sprintf(tp, "while(\n");
        write_in_file("Intermediate.txt", tp);
        advance();
        tempvar=expr();
        sprintf(tp, "%s)\n", tempvar);
        write_in_file("Intermediate.txt", tp);
        if(!match(DO))
            fprintf( stderr,"%d: Inserting missing do\n", yylineno );
        else
            advance();

       	sprintf(tp, "do{\n");
        write_in_file("Intermediate.txt", tp);
        stmt();
        sprintf(tp, "}\n");
        write_in_file("Intermediate.txt", tp);
        freename(tempvar);
        return;
    }

    if(match(BEGIN))
    {
        sprintf(tp, "begin{\n");
        write_in_file("Intermediate.txt", tp);
        advance();
        opt_stmts();
        return;
    }

    fprintf( stderr,"%d: Grammar mismatch\n", yylineno );
    exit(0);
}

void opt_stmts ()
{
    if(match(END))
    {
        advance();
    	write_in_file("Lexemes.txt","<END> ");
        sprintf(tp, "}end\n");
        write_in_file("Intermediate.txt", tp);
        return;
    }
    stmt_list();
    if(!match(END))
        fprintf( stderr,"%d: Inserting missing end\n", yylineno);
   	else
        advance();
	write_in_file("Lexemes.txt","<END> ");
    sprintf(tp, "}end\n");
    write_in_file("Intermediate.txt", tp);
}

char   *expr ()
{
    //relational
    // printf("expr\n");

    char * tempvar;
    char * tempvarRes;
    char * tempvar2;

    tempvar=expression();
    if(match(GT))
    {
        freename(tempvar);
        tempvarRes=newname();
        tempvar=newname();
        sprintf(tp, "%s = %s\n", tempvar, tempvarRes);
        write_in_file("Intermediate.txt", tp);
        advance();
        tempvar2=expression();
        sprintf(tp, "%s = %s > %s\n",tempvarRes,tempvar,tempvar2);
        write_in_file("Intermediate.txt", tp);
        freename(tempvar2);
        freename(tempvar);
        return tempvarRes;
    }
    else if (match(LT))
    {
        freename(tempvar);
        tempvarRes=newname();
        tempvar=newname();
        sprintf(tp, "%s = %s\n", tempvar, tempvarRes);
        write_in_file("Intermediate.txt", tp);
        advance();
        tempvar2=expression();
        sprintf(tp, "%s = %s < %s\n",tempvarRes,tempvar,tempvar2);
        write_in_file("Intermediate.txt", tp);
        freename(tempvar2);
        freename(tempvar);
        return tempvarRes;
    }
    else if (match(ET))
    {
        freename(tempvar);
        tempvarRes=newname();
        tempvar=newname();
        sprintf(tp, "%s = %s\n", tempvar, tempvarRes);
        write_in_file("Intermediate.txt", tp);
        advance();
        tempvar2=expression();
        sprintf(tp, "%s = %s == %s\n",tempvarRes,tempvar,tempvar2);
        write_in_file("Intermediate.txt", tp);
        freename(tempvar2);
        freename(tempvar);
        return tempvarRes;
    }
    else
    {
        return tempvar;
    }

}

char   *expression()
{
    /* expression -> term expression'
     * expression' -> PLUS term expression' |  epsilon
     */
    // printf("expression\n");

    char  *tempvar, *tempvar2;

    tempvar = term();
    while(1)
    {
        if(match(PLUS))
        {
            advance();
            tempvar2 = term();
            sprintf(tp, "%s += %s\n", tempvar, tempvar2 );
        	write_in_file("Intermediate.txt", tp);
            freename( tempvar2 );
        }
        else if(match(MINUS))
        {
            advance();
            tempvar2 = term();
            sprintf(tp, "%s -= %s\n", tempvar, tempvar2 );
        	write_in_file("Intermediate.txt", tp);
            freename( tempvar2 );
        }
        else
            break;
    }
    return tempvar;
}

char    *term()
{
    // printf("term\n");

    char  *tempvar, *tempvar2 ;

    tempvar = factor();
    while(1)
    {
        if( match( TIMES ) )
        {
            advance();
            tempvar2 = factor();
            sprintf(tp, "%s *= %s\n", tempvar, tempvar2 );
        	write_in_file("Intermediate.txt", tp);
            freename( tempvar2 );
        }
        else if (match (DIV))
        {
            advance();
            tempvar2 = factor();
            sprintf(tp, "%s /= %s\n", tempvar, tempvar2 );
        	write_in_file("Intermediate.txt", tp);
            freename( tempvar2 );
        }
        else
            break;
    }

    return tempvar;
}

char    *factor()
{
    // printf("factor\n");

    char *tempvar;

    if( match(NUM_OR_ID) )
    {
 /* Print the assignment instruction. The %0.*s conversion is a form of
  * %X.Ys, where X is the field width and Y is the maximum number of
  * characters that will be printed (even if the string is longer). I'm
  * using the %0.*s to print the string because it's not \0 terminated.
  * The field has a default width of 0, but it will grow the size needed
  * to print the string. The ".*" tells printf() to take the maximum-
  * number-of-characters count from the next argument (yyleng).
  */
    	write_in_file("Lexemes.txt","<NUM_OR_ID> ");
        sprintf(tp, "%s = %0.*s\n", tempvar = newname(), yyleng, yytext );
        write_in_file("Intermediate.txt", tp);
        advance();
    }
    // else if( match(LP) )
    // {
    //     advance();
    //     tempvar = expression();
    //     if( match(RP) )
    //         advance();
    //     else
    //         fprintf(stderr, "%d: Mismatched parenthesis\n", yylineno );
    // }
    else
 		fprintf( stderr, "%d: Number or identifier expected\n", yylineno );

    return tempvar;
}

