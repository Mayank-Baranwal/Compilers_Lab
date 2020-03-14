%{
# include "sqlquery.tab.h"
%}

%%
\n                              {printf("%s ",yytext ); yylineno++; return NEWLINE; }
SELECT                          {printf("<%s> ",yytext ); return SELECT;}
PROJECT                         {printf("<%s> ",yytext ); return PROJECT;}
CARTESIAN_PRODUCT               {printf("<%s> ",yytext ); return CARTESIAN_PRODUCT;} 
EQUI_JOIN                       {printf("<%s> ",yytext ); return EQUI_JOIN;}
"OR"                            {printf("<%s> ",yytext ); return OR;}
"AND"                           {printf("<%s> ",yytext ); return AND;}
"NOT"                           {printf("<%s> ",yytext ); printf("<%s> ",yytext ); return NOT;}
[0-9]+                          {printf("<%s> ",yytext ); return INT; }
'[A-Za-z_][0-9A-Za-z_]*'        {printf("<%s> ",yytext ); return STRING;}
"<"                             {printf("<%s> ",yytext ); return LT;}
">"                             {printf("<%s> ",yytext ); return GT;}
"<="                            {printf("<%s> ",yytext ); return LTE;}
">="                            {printf("<%s> ",yytext ); return GTE;}
"="                             {printf("<%s> ",yytext ); return EQUAL;}             
"<>"                            {printf("<%s> ",yytext ); return NE;} 
\(                             {printf("<%s> ",yytext ); return LP;}
\)                             {printf("<%s> ",yytext ); return RP;}
,                               {printf("<%s> ",yytext ); return COMMA;}
\.                              {printf("<%s> ",yytext ); return DOT;}
[A-Za-z_][0-9A-Za-z_]*          {printf("<%s> ",yytext ); return ID; }
[ \t]                           {/* no need to do anything for whitespaces */ }
.                               {printf("Unknown character %c\n", *yytext);}
%%
