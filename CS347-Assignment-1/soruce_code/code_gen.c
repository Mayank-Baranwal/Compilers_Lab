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

void stmt_list ()
{
    char * tempvar;
    while (1)
    {
        stmt();
        if(match(END))
            break;
        if( match( SEMI ) )
            advance();
        else
            fprintf( stderr,"%d: Inserting missing semicolon\n", yylineno );
    }
}


void stmt ()
{
    char * tempvar;

    if(match(EOI))
    	return;
    if(match(NUM_OR_ID))
    {
    	write_in_file("Lexemes.txt","<ID> ");
        int yyleng_temp=yyleng;
        char yytext_temp[50];
        strcpy(yytext_temp,yytext);
        advance();
        if(match(COLET))
        {
            advance();

            tempvar=expr();
            printf("%0.*s <- %s\n", yyleng_temp, yytext_temp, tempvar);
            freename(tempvar);
        }  
        return;
    }   
    if (match(IF))
    {
        printf("if(\n");
        advance();
        tempvar=expr();
        printf("%s\n", tempvar);
        printf(")\n");
        if(match(THEN))
        {
            printf("then{\n");
            advance();
            stmt();
            printf("}\n");
        }
        freename(tempvar);
        return;
    }
    if (match(WHILE))
    {
        printf("while(\n");
        advance();
        tempvar=expr();
        printf("%s\n", tempvar);
        printf(")\n");
        if(match(DO))
        {
            printf("do{\n");
            advance();
            stmt();
            printf("}\n");
        }
        freename(tempvar);
        return;
    }

    if(match(BEGIN))
    {
        printf("begin{\n");
        advance();
        opt_stmts();
        return;
    }

}

void opt_stmts ()
{
    if(match(END))
    {
        advance();
    	write_in_file("Lexemes.txt","<END> ");
        printf("}end\n");
        return;
    }
    stmt_list();
    if(match(END))
    {
        advance();
    	write_in_file("Lexemes.txt","<END> ");
        printf("}end\n");
    }
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
        printf("%s <- %s\n", tempvar, tempvarRes);
        advance();
        tempvar2=expression();
        printf("%s <-  %s > %s\n",tempvarRes,tempvar,tempvar2);
        freename(tempvar2);
        freename(tempvar);
        return tempvarRes;
    }
    else if (match(LT))
    {
        freename(tempvar);
        tempvarRes=newname();
        tempvar=newname();
        printf("%s <- %s\n", tempvar, tempvarRes);
        advance();
        tempvar2=expression();
        printf("%s <-  %s < %s\n",tempvarRes,tempvar,tempvar2);
        freename(tempvar2);
        freename(tempvar);
        return tempvarRes;
    }
    else if (match(ET))
    {
        freename(tempvar);
        tempvarRes=newname();
        tempvar=newname();
        printf("%s <- %s\n", tempvar, tempvarRes);
        advance();
        tempvar2=expression();
        printf("%s <-  %s = %s\n",tempvarRes,tempvar,tempvar2);
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
            printf("%s <- %s + %s\n", tempvar, tempvar, tempvar2 );
            freename( tempvar2 );
        }
        else if(match(MINUS))
        {
            advance();
            tempvar2 = term();
            printf("%s = %s - %s\n", tempvar, tempvar, tempvar2 );
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
            printf("%s <- %s * %s\n", tempvar, tempvar, tempvar2 );
            freename( tempvar2 );
        }
        else if (match (DIV))
        {
            advance();
            tempvar2 = factor();
            printf("%s <- %s / %s\n", tempvar, tempvar, tempvar2 );
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
        printf("%s <- %0.*s\n", tempvar = newname(), yyleng, yytext );
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

