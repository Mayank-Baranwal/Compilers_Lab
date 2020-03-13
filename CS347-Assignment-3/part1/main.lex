%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<stdbool.h>
	#include<string.h>

	FILE * remove_comments();
	
%}

%option noyywrap

%%
%%


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
}