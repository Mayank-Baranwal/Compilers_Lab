// #include "name.h"

char  *Names[] = { "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7" };
char  Register_Names[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'H', 'L' };
char  **Namep  = Names;

char  *newname(void)
{
    if( Namep >= &Names[ sizeof(Names)/sizeof(*Names) ] )
    {
        fprintf( stderr, "%d: Expression too complex\n", yylineno );
        exit( 1 );
    }

    return( *Namep++ );
}


void freename(char *s)
{
    if( Namep > Names )
    *--Namep = s;
    else
    fprintf(stderr, "%d: (Internal error) Name stack underflow\n",
                                yylineno );
}

char getRegister(char * s)
{
    for(int i=0;i<8;i++)
    {
        if(strcmp(Names[i],s)==0)
            return Register_Names[i];
    }
    return '\0';
}

char * uscore(char * s)
{
	bool flag=0;
	if(getRegister(tempvar)=='\0')
	{
		if(tempvar[0]>='0' && tempvar[0]<='9')
			flag=1;
	}
	char ans[40];
	int index=0;
	if(flag==0)
	{
		ans[index++]='_';
	}
	while(*s!='\0')
	{
		ans[index++]=*s;
	}
	ans[index]='\0';
	return ans;
}
