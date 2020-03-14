sqlquery:	sqlquery.lex sqlquery.y
		flex sqlquery.lex
		bison -d sqlquery.y
		gcc -o exec -lfl sqlquery.tab.c lex.yy.c 
		
clean:	exec sqlquery.tab.c sqlquery.tab.h lex.yy.c
		rm exec sqlquery.tab.c sqlquery.tab.h lex.yy.c