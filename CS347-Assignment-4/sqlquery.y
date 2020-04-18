%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
int yyparse();
int yylineno;
int yylinenumber;
char* yytext;
int yyleng;
void yyerror(char* s);
char list[200][200];
int currentValue;
%}

%code requires 
{
    #include "helper.h"
}

%union 
{
    char charText[100];
    int integerValue;

    struct OrList orList;

    struct AndEntry andEntry;
    struct AndList andList;
    
    struct 
    {
        char* table;
        char* colName;
    } colattr;

    struct 
    {
        int operandType;
    } opType;
}

// token declaration

%token SELECT PROJECT CARTESIAN_PRODUCT EQUI_JOIN
%token OR AND NOT
%token INT STRING ID
%token LT GT LTE GTE EQUAL NE
%token LP RP COMMA DOT NEWLINE

%type <orList> condition
%type <andList> temp_cond1
%type <andEntry> temp_cond2
%type <andEntry> expression
%type <opType> operator
%type <colattr> column
%type <integerValue> INT;
%type <charText> ID;
%type <charText> STRING;


%%
stmts : stmt NEWLINE stmts
        | stmt
        | error NEWLINE {printf("Invalid Syntax: Error in line number %d\n",yylineno-1);printf("-----------------------------------------\n-----------------------------------------\n\n");} stmts 
        ;


stmt:  SELECT LT  GT LP ID RP
    {
        yylinenumber = yylineno;
        char fileName[200];
        memset(fileName,0,200);
        sprintf(fileName,"testfile/%s.csv",$5);
        if(is_table_present($5))
        {
            FILE* fp = fopen(fileName,"r");
            char str[1000];
            fgets(str, 1000, fp);
            printf("%s", str);
            fgets(str, 1000, fp);
            OrList condition;
            condition.head = condition.tail = NULL;
            int numOfRecords = 0;
            while (fgets(str, 1000, fp))
            {
                sscanf(str, "%[^\n]", str);
                int returnResult = computingCondSelect(condition, str, $5);
                if (returnResult == -1)
                {
                    break;
                }
                else if (returnResult)  
                {
                    printf("%s\n", str);
                    numOfRecords++;
                }
            }
            printf("Total number of records : %d\n", numOfRecords);
            fclose(fp);
        }
        else
        {
            printf("Could not find Table %s.\n",$5);
        }
        printf("-----------------------------------------\n-----------------------------------------\n\n");        
    }
    |SELECT LT condition GT LP ID RP
    {
        yylinenumber = yylineno;
        char fileName[200];
        memset(fileName,0,200);
        sprintf(fileName,"testfile/%s.csv",$6);
        if(is_table_present($6))
        {
            FILE* fp = fopen(fileName,"r");
            char str[1000];
            fgets(str, 1000, fp);
            printf("%s", str);
            fgets(str, 1000, fp);
            int numOfRecords = 0;
            while (fgets(str, 1000, fp))
            {
                sscanf(str, "%[^\n]", str);
                int returnResult = computingCondSelect($3, str, $6);
                if (returnResult == -1)
                {
                    break;
                }
                else if (returnResult)  
                {
                    numOfRecords++;
                    printf("%s\n", str);
                }
            }

            printf("Total number of records : %d\n", numOfRecords);
            fclose(fp);
        }
        else
        {
            printf("Could not find Table %s.\n",$6); 
        }
        printf("-----------------------------------------\n-----------------------------------------\n\n");
    }
    | PROJECT LT attribute_list GT LP ID RP
    {
        yylinenumber = yylineno;
        if (is_table_present($6)) 
        {
            projection(list, currentValue, $6);        
        }
        else 
        {
            fprintf(stdout, "Could not find Table %s.\n", $6);
        }
        printf("-----------------------------------------\n-----------------------------------------\n\n");
    }
    | LP ID RP CARTESIAN_PRODUCT LP ID RP       
    {
        yylinenumber = yylineno;
        cartesian_product($2, $6);
        printf("-----------------------------------------\n-----------------------------------------\n\n");
    }
    | LP ID RP EQUI_JOIN LT condition GT LP ID RP       
    {
        yylinenumber = yylineno;
        int numOfRecords=0;
        if (!is_table_present($2)) 
        {
            fprintf(stdout, "Table %s not present\n", $2);
        }
        else if (!is_table_present($9)) 
        {
            fprintf(stdout, "Table %s not present\n", $9);
        }
        else 
        {
            int x = attachTable($2, $9, &($6));
            if (x == 1) 
            {
                numOfRecords = showEquiJoin($2, $9, &($6));
            }
            printf("Total number of records : %d\n", numOfRecords);
        }
        printf("-----------------------------------------\n-----------------------------------------\n\n");
    }
    | %empty
    ;

// OR has lower precedence than AND
condition: temp_cond1 OR condition
    {
        $$ = combineOrList($3, $1); 
    } 
    | temp_cond1  
    {
        $$.tail = malloc(sizeof(AndList));
        $$.head = $$.tail;
        memcpy($$.head, &$1, sizeof (AndList));
    }
    ;

// AND has lower precedence than NOT
temp_cond1: temp_cond2 AND temp_cond1
		    {
		        $$ = combineAndList($3, $1); 
		    }
		    | temp_cond2
		    {
		        $$.tail = malloc(sizeof(AndEntry));
		        $$.head = $$.tail;
		        $$.nextPtr = NULL;
		        memcpy($$.head, &$1, sizeof (AndEntry));
		    }
			;

temp_cond2 : NOT LP condition RP
		    {   
                $$.findInteger1 = 0;
		        $$.findInteger2 = 0;
		        $$.str1 = NULL;
		        $$.str2 = NULL;

                $$.tableEntry1 = NULL;
                $$.tableEntry2 = NULL;
                $$.columnEntry1 = NULL;
                $$.columnEntry2 = NULL;
                $$.operation = 0;

		        $$.nextPtr = NULL;
		        $$.isCondition = 1;
		        $$.isNegation = 1;
		        $$.nestedCond = malloc(sizeof(OrList));
		        memcpy($$.nestedCond, &$3, sizeof (OrList));
		    }
            | expression
            ;

expression: column operator STRING
			{
		        if($1.table)
                { 
                    $$.tableEntry1 = malloc(100); 
                    memset($$.tableEntry1, 0, 100); 
                    sprintf($$.tableEntry1, "%s", $1.table);
                }
		        else
                { 
                    $$.tableEntry1 = $1.table;                    
                }
		        
                $$.tableEntry2 = NULL;
		        $$.columnEntry1 = malloc(100);  
                memset($$.columnEntry1, 0, 100);
		        $$.columnEntry2 = NULL;
		        sprintf($$.columnEntry1, "%s", $1.colName);
		        $$.operation = $2.operandType;
		        
                $$.findInteger1 = 0;
		        $$.findInteger2 = 0;
		        $$.str1 = NULL;
		        $$.str2 = malloc(100); memset($$.str2, 0, 100);
		        sprintf($$.str2, "%s", $3);
		        
                $$.nextPtr = NULL;
				$$.isNegation = 0;        
		        $$.isCondition = 0;
		        $$.nestedCond = NULL;
			}
		    | STRING operator column
		    {
		        if($3.table)
                { 
                    $$.tableEntry1 = malloc(100); 
                    memset($$.tableEntry1, 0, 100); 
                    sprintf($$.tableEntry1, "%s", $3.table);
                }
		        else 
                { 
                    $$.tableEntry1 = $3.table; 
                }
		        
                $$.tableEntry2 = NULL;
                $$.columnEntry1 = malloc(100);  
                memset($$.columnEntry1, 0, 100);
                $$.columnEntry2 = NULL;
                sprintf($$.columnEntry1, "%s", $3.colName);
                $$.operation = makeComplement($2.operandType);

		        $$.findInteger1 = 0;
		        $$.findInteger2 = 0;
		        $$.str1 = NULL;
		        $$.str2 = malloc(100);
                memset($$.str2, 0, 100);
		        sprintf($$.str2, "%s", $1);
		        
                $$.nextPtr = NULL;
				$$.isNegation = 0;        
		        $$.isCondition = 0;
		        $$.nestedCond = NULL;
		    }
		    | column operator column
		    {
		        if($1.table)
                { 
                    $$.tableEntry1 = malloc(100); 
                    memset($$.tableEntry1, 0, 100); 
                    sprintf($$.tableEntry1, "%s", $1.table);
                }
		        else 
                {
                    $$.tableEntry1 = $1.table;
                }
		        if($3.table)
                { 
                    $$.tableEntry2 = malloc(100); 
                    memset($$.tableEntry2, 0, 100); 
                    sprintf($$.tableEntry2, "%s", $3.table);
                }
		        else 
                { 
                    $$.tableEntry2 = $3.table; 
                }
		        $$.columnEntry1 = malloc(100);  memset($$.columnEntry1, 0, 100);
		        $$.columnEntry2 = malloc(100);  memset($$.columnEntry2, 0, 100); 
		        sprintf($$.columnEntry1, "%s", $1.colName);
		        sprintf($$.columnEntry2, "%s", $3.colName);
		        $$.operation = $2.operandType;
		        
                $$.findInteger1 = 0;
		        $$.findInteger2 = 0;
		        $$.str1 = NULL;
		        $$.str2 = NULL;
		        
                $$.nextPtr = NULL;
				$$.isNegation = 0;        
		        $$.isCondition = 0;
		        $$.nestedCond = NULL;    
		    }
		    | column operator INT
		    {
		        if($1.table)
                { 
                    $$.tableEntry1 = malloc(100); 
                    memset($$.tableEntry1, 0, 100); 
                    sprintf($$.tableEntry1, "%s", $1.table);
                }
		        else 
                { 
                    $$.tableEntry1 = $1.table; 
                }
		        $$.tableEntry2 = NULL;
		        $$.columnEntry1 = malloc(100);  memset($$.columnEntry1, 0, 100);
		        $$.columnEntry2 = NULL;
		        sprintf($$.columnEntry1, "%s", $1.colName);
		        $$.operation = $2.operandType;
		        $$.findInteger1 = 0;
		        $$.findInteger2 = 1;
		        $$.secondVal = $3;
		        $$.str1 = NULL;
		        $$.str2 = NULL;
		        $$.nextPtr = NULL;
				$$.isNegation = 0;        
		        $$.isCondition = 0;
		        $$.nestedCond = NULL;
		    }
		    | INT operator column
		    {
		        if($3.table)
                {
                    $$.tableEntry1 = malloc(100); 
                    memset($$.tableEntry1, 0, 100); 
                    sprintf($$.tableEntry1, "%s", $3.table);
                }
		        else 
                { 
                    $$.tableEntry1 = $3.table; 
                }
		        $$.tableEntry2 = NULL;
		        $$.columnEntry1 = malloc(100);  memset($$.columnEntry1, 0, 100);
		        $$.columnEntry2 = NULL;
		        sprintf($$.columnEntry1, "%s", $3.colName);
		        $$.operation = makeComplement($2.operandType);

		        $$.findInteger1 = 0;
		        $$.findInteger2 = 1;
		        $$.secondVal = $1;
		        $$.str1 = NULL;
		        $$.str2 = NULL;

		        $$.nextPtr = NULL;
				$$.isNegation = 0;        
		        $$.isCondition = 0;
		        $$.nestedCond = NULL;
		    }
		    | LP condition RP
		    {
                $$.findInteger1 = 0;
                $$.findInteger2 = 0;
                $$.str1 = NULL;
                $$.str2 = NULL;   

		        $$.tableEntry1 = NULL;
		        $$.tableEntry2 = NULL;
		        $$.columnEntry1 = NULL;
		        $$.columnEntry2 = NULL;
		        $$.operation = 0;

		        $$.nextPtr = NULL;
				$$.isNegation = 0;        
		        $$.isCondition = 1;
		        $$.nestedCond = malloc(sizeof(OrList));
		        memcpy($$.nestedCond, &$2, sizeof (OrList));
		    }
		    ;

operator: LT {$$.operandType = 1;}
        | GT {$$.operandType = 2;}
        | LTE {$$.operandType = 3;}
        | GTE {$$.operandType = 4;}
        | EQUAL {$$.operandType = 5;}
        | NE {$$.operandType = 6;}
        ;

attribute_list: column COMMA attribute_list
                {
                    sprintf(list[currentValue], "%s", $1.colName);
                    currentValue++;
                }
                | column
                {
                    memset(list, 0, 10000);
                    currentValue = 0;
                    sprintf(list[0], "%s", $1.colName);
                    currentValue++;
                }
                ;

column: ID DOT ID 
		{
	        $$.table = malloc(100);
	        $$.colName = malloc(100);
	        memset($$.colName, 0, 100);
	        memset($$.table, 0, 100);
	        sprintf($$.table, "%s", $1);
	        sprintf($$.colName, "%s", $3);
	    }
	    | ID  
	    {
	        $$.table = NULL;
	        $$.colName = malloc(100);
	        memset($$.colName, 0, 100);
	        sprintf($$.colName, "%s", $1);       
	    }
		;

%%
int main(int argc, char **argv)
{
  yyparse();
}

void yyerror(char *s){}