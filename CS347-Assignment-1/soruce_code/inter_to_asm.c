// #include <stdio.h>
// #include <stdlib.h>
// #include <error.h>
// #include <string.h>
// #include <ctype.h>

FILE* fp1,*fp2;

void convert(){
	fp1 = fopen("Intermediate.txt", "r");
	fp2 = fopen("Assembly_Code.txt","w");
	// while (fp != NULL){
	char line[100];

	const char delim[] = " ";
	while (fgets (line, sizeof line, fp1) != NULL){
		// printf("%s", line);
		char word[10][100], temp[100];

		int cnt = 0;
		int index=0;
		while(line[index]!='\n')
		{
			temp[index]=line[index];
			index++;
		}
		temp[index]='\0';

		index=0;
		int cnt2=0;
		while (temp[index] != '\0'){
			// printf("%c", temp[index] );

			if(temp[index]!=' ')
			{
				word[cnt][cnt2++]=temp[index];
				index++;
			}
			else
			{
				word[cnt][cnt2]='\0';
				cnt++;
				cnt2=0;
				while(temp[index]==' ')
					index++;
			}
		}
		word[cnt][cnt2]='\0';
		cnt++;
		for (int i=0; i<cnt; i++){
			printf("%s ", word[i]);
		}
		printf("\n");

		printf("%d\n\n", cnt);

		
	}

	// }
	// }
}
