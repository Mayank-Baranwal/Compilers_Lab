#include "lex.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

char* yytext = ""; /* Lexeme (not '\0'
                      terminated)              */
int yyleng   = 0;  /* Lexeme length.           */
int yylineno = 0;  /* Input line number        */

int lex(){

   static char input_buffer[1024];
   char        *current;

   current = yytext + yyleng; /* Skip current
                                 lexeme        */

   while(1){       /* Get the next one         */
      while(!*current ){
         /* Get new lines, skipping any leading
         * white space on the line,
         * until a nonblank line is found.
         */

         current = input_buffer;
         if(!gets(input_buffer)){
            *current = '\0' ;

            return EOI;
         }
         ++yylineno;
         while(isspace(*current))
            ++current;
      }
      for(; *current; ++current){
         /* Get the next token */
         yytext = current;
         yyleng = 1;
         switch( *current ){
           case ';':
            return SEMI;
           case '+':
            return PLUS;
           case '-':
            return MINUS;
           case '*':
            return TIMES;
           case '/':
            return DIV;
           case '(':
            return LP;
           case ')':
            return RP;
           case '>':
            return GT;
           case '<':
            return LT;
           case '=':
            return ET;
           case '\n':
           case '\t':
           case ' ' :
            break;
           default:
            if(*current == ':' && *(current+1) == '=')
            {
              current += 2;
              yyleng = current - yytext;
              return COLET;
            }
            else if(!isalnum(*current))
               fprintf(stderr, "Not alphanumeric <%c>\n", *current);
            else{
                char temp[50];
                int i=0;
               while(isalnum(*current))
               {
                temp[i++]= (*current);
                ++current;
               }
               temp[i]='\0';
               yyleng = current - yytext;
               if(strcmp(temp,"if")==0)
                    return IF;
               if(strcmp(temp,"then")==0)
                    return THEN;
               if(strcmp(temp,"while")==0)
                    return WHILE;
               if(strcmp(temp,"do")==0)
                    return DO;
               if(strcmp(temp,"begin")==0)
                    return BEGIN;
               if(strcmp(temp,"end")==0)
                    return END;    
               return NUM_OR_ID;
            }
            break;
         }
      }
   }
}


static int Lookahead = -1; /* Lookahead token  */

int match(int token){
   /* Return true if "token" matches the
      current lookahead symbol.                */

   if(Lookahead == -1)
      Lookahead = lex();

   return token == Lookahead;
}

void advance(void){
/* Advance the lookahead to the next
   input symbol.                               */

    Lookahead = lex();
}