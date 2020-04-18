%{
# include "sqlquery.tab.h"
%}

%%
(?i:SELECT)                          { return SELECT; }
(?i:PROJECT)                         { return PROJECT; }
(?i:CARTESIAN_PRODUCT)               { return CARTESIAN_PRODUCT; }
(?i:EQUI_JOIN)                       { return EQUI_JOIN; }
(?i:OR)                            { return OR; }
(?i:AND)                           { return AND; }
(?i:NOT)                           { return NOT; }
[0-9]+                          { yylval.integerValue = atoi(yytext); return INT; }
\<                             {return LT;}
\>                             {return GT;}
\<\=                            {return LTE;}
\>\=                            {return GTE;}
\=                             {return EQUAL;}             
\!\=                            {return NE;}           
,                               { return COMMA; }
\.                              { return DOT; }
\'[0-9A-Za-z_,]*\'                { sprintf(yylval.charText,"%s", yytext); return STRING; }
\"[0-9A-Za-z_,]*\"                { sprintf(yylval.charText,"%s", yytext); return STRING; }
[(]                             { return LP; }
[)]                             { return RP; }
[A-Za-z_][0-9A-Za-z_]*          { sprintf(yylval.charText,"%s", yytext); return ID; }
\n                              { yylineno++; return NEWLINE; }
[ \t]                           { /* ignore whitespace */ }
.                               { printf("Unknown character %c\n", *yytext); }
%%