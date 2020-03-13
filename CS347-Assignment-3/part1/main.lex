%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<stdbool.h>
	#include<string.h>

	FILE * remove_comments();
	void line_increment(void);
    void put_class_in_array(char*);
    int exists_class(char*);
    void print_classes();
    void verify_obj(char*);	

    char class_to_add[200];
    char class_list[10000][200];
    int class_count=0;
    int class_total = 0;
    int inherited_class_total = 0;
    int constructor_total=0;
    int op_overload_total=0;
    int object_total=0;
    int class_in_line = 0;
    int inherited_class_in_line = 0;
    int constructor_in_line = 0;
    int operator_in_line = 0;
    int object_in_line=0;
%}

st   [A-Za-z_]
wd   [A-Za-z_0-9]
dec   public|private|protected
OP \+=|-=|\*=|\/=|%=|\^=|&=|\|=|<<|>>|>>=|<<=|==|!=|<=|>=|<=>|&&|\|\||\+\+|--|\,|->\*|\\->|\(\s*\)|\[\s*\]|\+|-|\*|\/|%|\^|&|\||~|!|=|<|>


%option noyywrap
%x CLS_A CLS_B CLS_C CLS_D CNST_A CNST_B CNST_C DST_A DST_B DST_C
%%

[^A-Za-z_]operator" "*{OP}" "*([^\{\;\n]*)[\n\{]     { operator_in_line = 1; if(yytext[yyleng-1]=='\n'){line_increment();}}

[^0-9a-zA-Z_]class[\t ]+  	{ if(yytext[0] == '\n'){line_increment();} BEGIN(CLS_A); }
<CLS_A>{st}{wd}*             { memset(class_to_add, 0, sizeof(class_to_add)) ; snprintf(class_to_add, 200, "%s", yytext); BEGIN(CLS_B) ; }
<CLS_B>[ \t]                 { BEGIN(CLS_B) ; } 
<CLS_B>\n                    { class_in_line = 1; line_increment(); if(exists_class(class_to_add)==0){put_class_in_array(class_to_add);} BEGIN(INITIAL);}
<CLS_B>"{"                   { class_in_line = 1; if(exists_class(class_to_add)==0){put_class_in_array(class_to_add);} /*printf("%s-was here\n", class_to_add);*/ BEGIN(INITIAL); }
<CLS_B>":"                   { BEGIN(CLS_C); }  
<CLS_B>[^\n{:]               { put_class_in_array(class_to_add); BEGIN(INITIAL);}
<CLS_C>[ \t]                 { BEGIN(CLS_C); }
<CLS_C>{dec}                 { BEGIN(CLS_C); }
<CLS_C>{st}{wd}*             { BEGIN(CLS_D); }
<CLS_D>[ \t]                 { BEGIN(CLS_D); }
<CLS_D>","                   { BEGIN(CLS_C); }
<CLS_D>\n                    { inherited_class_in_line = 1, class_in_line = 1; line_increment(); if(exists_class(class_to_add)==0){put_class_in_array(class_to_add);} BEGIN(INITIAL);}
<CLS_D>"{"                   { class_in_line = 1, inherited_class_in_line = 1; if(exists_class(class_to_add)==0){put_class_in_array(class_to_add);} BEGIN(INITIAL); }
<CLS_D>[^,\n{ ]              { put_class_in_array(class_to_add); BEGIN(INITIAL);}


[~]                       { BEGIN(DST_A);}
<DST_A>{st}{wd}*             { BEGIN(DST_A); }
<DST_A>[ \t]                 ;
<DST_A>[(]                   { BEGIN(DST_B);}
<DST_B>[^)]                  ;
<DST_B>[)]                   { BEGIN(DST_C);}
<DST_C>[ ]                   { BEGIN(DST_C);}
<DST_C>[{:]                  { BEGIN(INITIAL);}
<DST_C>\n                    { line_increment(); BEGIN(INITIAL);}
<DST_C>[^\n{:]               { BEGIN(INITIAL);} 


{st}{wd}*[ ]*[(]          { memset(class_to_add, 0, sizeof(class_to_add)) ; sscanf(yytext, "%[A-Za-z0-9_]s", class_to_add); BEGIN(CNST_B) ; /* printf("%s-const-%d-%s-\n", yytext, yylineno, class_to_add); */}
<CNST_A>[ \t]                 { BEGIN(CNST_A);}
<CNST_A>[(]                   { BEGIN(CNST_B);}
<CNST_B>[^)]                  { BEGIN(CNST_B);}
<CNST_B>[)]                   { BEGIN(CNST_C);}
<CNST_C>[ \t]                 { BEGIN(CNST_C);}
<CNST_C>[{:]                  { if (exists_class(class_to_add)){ constructor_in_line = 1; /* printf("######%s#######", class_to_add);*/ } /* printf("%s-was here\n", class_to_add);*/ BEGIN(INITIAL);}
<CNST_C>\n                    { if (exists_class(class_to_add)) {constructor_in_line = 1; /* printf("######%s#######", class_to_add);*/ }line_increment(); /*printf("%s-was here\n", class_to_add);*/ BEGIN(INITIAL);}
<CNST_C>[^\n{:]               { BEGIN(INITIAL);} 

.                         ;    
\n                        { line_increment();}

// {st}{wd}*[*]*[ ]+[*]*[A-Za-z0-9_,][A-Za-z0-9_,\.\[\] ()]*[^\n;<>]*;  {/* printf("%s\n", yytext); */ verify_obj(yytext);}      
{st}{wd}*[*]*[ ]+[*]*[A-Za-z0-9_,][A-Za-z0-9_,\.\[\] ()]*;  {printf("%s\n", yytext); verify_obj(yytext);}      



%%

int exists_class(char *class_name){
    int cls=0;
    while (cls < class_count){
        if(strcmp(class_list[i], class_name) == 0)					//May not work, check earlier
    		return 1;
    	cls++;
    }
    return 0;
}

void put_class_in_array(char* class_to_add){
    class_count++;
    snprintf(class_list[class_count-1],  200, "%s", class_to_add);
}

// void verify_obj(char *class_value){
//     char class_title[250];
//     memset(class_title, 0, sizeof(class_title));
//     sscanf(class_value, "%s", class_title);
//     int len = strlen(class_title);
//     while(class_title[len-1] == '*') {
//         class_title[len-1] = '\0';
//         len--;
//     }
//     if(strstr(class_value, "operator") && strstr(class_value, "{")){
//         operator_in_line=1;
//         return;
//     }
//     if(exists_class(class_title)){
//         object_in_line = 1;
//         //printf("%s\n", class_value);
//     }
// }

void verify_obj(char *class_value){	
    char class_title[200];
    bzero(class_title, sizeof(class_title));
    sscanf(class_value, "%s", class_title);
    int len = strlen(class_title);								//Remove * from class
    while(class_title[len-1] == '*') {
        class_title[len-1] = '\0';
        len--;
    }
    // if(strstr(class_value, "operator") && strstr(class_value, "{")){
    //     operator_in_line=1;
    //     return;
    // }
    if(exists_class(class_title)){
        object_in_line = 1;
    }
}


void line_increment(){
    op_overload_total += operator_in_line;
    class_total += class_in_line;
    object_total += object_in_line;
    inherited_class_total += inherited_class_in_line;
    constructor_total += constructor_in_line;
    
    operator_in_line = 0;
    class_in_line = 0;
    object_in_line = 0;
    inherited_class_in_line = 0;
    constructor_in_line = 0;
}


void print_classes(){
    int cls = 0;
    char tp[200];
    printf("Printing class_list : \n");
    while (cls < class_count){
        strcpy(tp, class_list[i]);
        printf("%s ", tp);
    	cls++;
    }
    printf("\n");
}


FILE * remove_comments()
{
	FILE * fil = fopen("input.txt","r");
	if (fil == NULL)
	{
		printf("File doesn't exist in the current directory\n");
		exit(0);
	}
	FILE * temp = fopen("temp.txt","w");

	bool is_string=false;
	bool single_line_comment=false;
	bool multi_line_comment=false;

	size_t val = 2000;
	char * line=malloc(2000);
	memset(line,0,sizeof(line));
	char ch=' ';
	while(getline(&line,&val,fil)!=-1)
	{
		int len=strlen(line);
		if(line[len-1]=='\n')
			len--;
		single_line_comment=false;
		is_string=false;
		for(int i=0;i<len;i++)
		{
			if(is_string == true && line[i-1]!= '\\' && line[i] == '\"')
			{
				is_string = false;
				line[i-1]= ch;
				line[i]= ch;
			}
			else if (is_string == true)
			{
				line[i-1]=ch;
			}
			else if (multi_line_comment == true && line[i]=='*' && i+1<len && line[i+1]=='/' )
			{
				multi_line_comment= false;
				line[i]=ch;
				i++;
				line[i]=ch;
			}
			else if (is_string == true)
			{
				line[i-1]=ch;
			}
			else if (single_line_comment == true || multi_line_comment == true)
			{
				line[i]=ch;
			}
			else if (( i==0 || line[i-1]!= '\\') && line[i]=='\"')
			{
				is_string= true;
			}
			else if (line[i]=='/' && i+1<len && line[i+1]=='*')
			{
				multi_line_comment = true;
				line[i]=ch;
				i++;
				line[i]=ch;
			}
			else if (line[i]=='/' && i+1<len && line[i+1]=='/')
			{
				single_line_comment = true;
				line[i]=ch;
				i++;
				line[i]=ch;
			}
		}
		fprintf(temp,"%s",line);
		memset(line,0,sizeof(line));
	}

	return temp;
}

int main()
{
	FILE * fin_ptr = remove_comments();
	yyin = fin_ptr;
	yylex();
}