
#include "helper.h"

#define max_length 1000

AndList combineAndList(struct AndList and_condition, struct AndEntry expr)
{
    AndEntry* elem = malloc( sizeof(AndEntry) );
    memcpy(elem, &expr, sizeof(AndEntry));

    and_condition.tail -> nextPtr = elem;
    and_condition.tail = elem;
    return and_condition;
}

OrList combineOrList(struct OrList or_condition, struct AndList and_condition)
{
    AndList* list = malloc( sizeof(AndList) );
    memcpy(list, &and_condition, sizeof(AndList));

    or_condition.tail -> nextPtr = list;
    or_condition.tail = list;
    return or_condition;
}

void ShowList(struct OrList or_condition)
{
    AndList* list = or_condition.head;
    while(list)
    {
        AndEntry* temp = list -> head;
        while(temp)
        {
            if(temp -> columnEntry1) printf("%s; ", temp -> columnEntry1);
            else printf("%s; ", temp -> columnEntry2);
            temp = temp -> nextPtr;
        }
        printf("\n");
        list = list -> nextPtr;
    }
}


char *getType(char *tbl, int indCol){
    char tbltmp[400];
    memset(tbltmp,0,400);
    sprintf(tbltmp,"testfile/%s.csv",tbl);
    FILE* file = fopen(tbltmp, "r");
    char str[2000];
    char *tok;
    const char *s = ",";
    int j = 0,i=0;
    for(i=0;i<2;i++) fgets(str, 2000, file);
    sscanf(str, "%[^\n]", str);
    tok = strtok(str, s);
    char data_type[10];
    memset(data_type, 0, 10);
    while( tok != NULL ) {
        if (indCol == j) {
            fclose(file);
            return tok;
        }
        tok = strtok(NULL, s);
        j++;  
    }    
    fclose(file);
    return NULL;
}


char *returnValue(char *str, int indCol) {
    char *tok;
    const char *s = ",";
    tok = strtok(str, s);
    int i = 0;
    while( tok != NULL ) {
        if (indCol != i){
            tok = strtok(NULL, s);
            i++; 
            continue;
        }
        return tok;        
    }
}

int columnNumber(char *tbl, char *col) { // tbl already .csv appended
    char tbltmp[400];
    memset(tbltmp,0,400);
    sprintf(tbltmp,"testfile/%s.csv",tbl);
    FILE* file = fopen(tbltmp, "r");
    char str[2000];
    char *tok;
    const char *s = ",";
    int j = 0;
    fgets(str, 2000, file);
    sscanf(str, "%[^\n]", str);
    tok = strtok(str, s);
    while( tok != NULL ) {
        // printf("col - %s %d %d\n", tok, strlen(tok), strlen(col));
        if (strcmp(tok, col) != 0) {
            j++;
            tok = strtok(NULL, s);
            continue;
        }
        fclose(file);
        return j;
    }    
    fclose(file);
    return -1; 
}

int showEquiJoin(char *tableEntry1, char *tableEntry2, struct OrList *conditions) {
    /*char table1Name[400];
    memset(table1Name, 0, 400);
    sprintf(table1Name, "testfile/%s.csv", tableEntry1);
    char table2Name[400];
    memset(table2Name, 0, 400);
    sprintf(table2Name, "testfile/%s.csv", tableEntry2);*/
    
    char file_name[2][400];
    memset(file_name,0,sizeof(file_name));
    sprintf(file_name[0],"testfile/%s.csv",tableEntry1);
    sprintf(file_name[1],"testfile/%s.csv",tableEntry2);
    FILE* file1 = fopen(file_name[0],"r");
    FILE* file2 = fopen(file_name[1],"r");

    char str1[2000];
    fgets(str1, 2000, file1);
    sscanf(str1, "%[^\n]s", str1);
    char *token1;
    const char *s = ",";
    token1 = strtok(str1, s);
    while (token1 != NULL) {
        printf( "%s.%s, ", tableEntry1, token1);
        token1 = strtok(NULL, s);        
    }
    char str2[2000];
    fgets(str2, 2000, file2);
    char *token2;
    token2 = strtok(str2, s);
    while (token2 != NULL) {
        printf( "%s.%s", tableEntry2, token2);
        token2 = strtok(NULL, s);
        if (token2 != NULL) printf(", ");        
    }
    fgets(str1, 2000, file1);
    fgets(str2, 2000, file2);
    // printf("%s", str1);
    // printf(",%s", str2);
    fclose(file2);
    int numOfRecords = 0;
    while(fgets(str1, 2000, file1)) {
        sscanf(str1, "%[^\n]s", str1);

        file2 = fopen(file_name[1],"r");
        /*fgets(str2, 2000, file2);
        fgets(str2, 2000, file2);*/
        int k=0;
        for(k=0;k<2;k++) fgets(str2, 2000, file2);
        while (fgets(str2, 2000, file2)) {
            if (computingCondEquiJoin(*conditions, str1, str2, tableEntry1, tableEntry2)) {
                printf("%s", str1);
                printf(",%s", str2);
                numOfRecords++;
            };
        } 
        fclose(file2);     
    }
    fclose(file1);
    return numOfRecords;
}

int computingCondSelect(struct OrList condition, char *str, char* tblTitle){
    AndList* tp = condition.head;
    if(tp){
        int result = 0;
        while(tp){
            int val = 1;
            AndEntry* tp2 = tp->head;
            while(tp2){
                char cpyStr[2000];
                memset(cpyStr, 0, 2000);
                sprintf(cpyStr, "%s", str);
                int ret;
                if (tp2->isCondition != 1) {
                    ret = comparatorSelect(*tp2, cpyStr, tblTitle);
                }
                else {
                    ret = computingCondSelect(*(tp2->nestedCond), cpyStr, tblTitle);
                }
                if(ret != -1){
                    if(tp2->isCondition ==1 && tp2->isNegation){
                        ret = ret==0?1:0; 
                    }
                    val = val & ret;
                    tp2 = tp2->nextPtr;
                }
                else
                    return -1;
            }
            if(val!=1){
                result = result | val;
                tp = tp->nextPtr;
            }
            else
                return 1;
        }
        return result;
    }
    else
        return 1;
}

int makeComplement(int oper) {
    if (oper > 4)
        return oper;

    if (oper % 2 == 0)
        return oper-1;
    else
        return oper+1;     
}


int operandComparison(int num1, int num2, int oper) {
    switch(oper){
        case 6: return num1 != num2; break;
        case 5: return num1 == num2; break;
        case 4: return num1 >= num2; break;
        case 3: return num1 <= num2; break;
        case 2: return num1 > num2; break;
        case 1: return num1 < num2; break;
        default: return 1;
    }
 
}

int operandComparisonString(char *str1, char *str2, int oper){
    // printf("--%s--%s--", str1, str2);
    int returnType = strcmp(str1, str2);
    /*if (oper == 1 && returnType < 0)
        return 1;
    else if (oper == 2 && returnType > 0)
        return 1;
    else if (oper == 3 && returnType <= 0)
        return 1;
    else if (oper == 4 && returnType >= 0)
        return 1;
    else if (oper == 5 && returnType == 0)
        return 1;
    else if (oper == 6 && returnType != 0)
        return 1;
    else
        return 0;*/
    
    if((oper == 1 && returnType < 0)||(oper == 2 && returnType > 0)||(oper == 3 && returnType <= 0)||(oper == 4 && returnType >= 0)||(oper == 5 && returnType == 0)||(oper == 6 && returnType != 0)) return 1;
    else return 0;
}
 
int comparatorSelect(struct AndEntry unit, char *str1, char* tblTitle) {
    if (unit.findInteger2) { // col oper INT
        // printf("I'm %s %s here\n", tblTitle, unit.columnEntry1);
        if(unit.tableEntry1 != NULL && strcmp(unit.tableEntry1, tblTitle)!=0){
            printf("Error: Table %s exists instead of table %s\n", tblTitle, unit.tableEntry1);
            return -1;
        }
        int indCol = columnNumber(tblTitle, unit.columnEntry1);
        if (indCol == -1) {
            printf("Error: Column %s does not exist in table %s\n", unit.columnEntry1, tblTitle);
            return -1;
        }
        char *data_type = getType(tblTitle, indCol);
        if(strcmp(data_type,"int") != 0){
            printf("Error: Data types incompatible\n");
            return -1;
        }
        unit.firstVal = atoi(returnValue(str1, indCol));
        return operandComparison(unit.firstVal, unit.secondVal, unit.operation);
    }
    else if (unit.findInteger1) {
        if(unit.tableEntry2 != NULL && strcmp(unit.tableEntry2, tblTitle)!=0){
            printf("Error: Table %s exists instead of table %s\n", tblTitle, unit.tableEntry2);
            return -1;
        }
        int indCol = columnNumber(tblTitle, unit.columnEntry2);
        if (indCol == -1) {
            printf("Error: Column %s does not exist in table %s\n", unit.columnEntry2, tblTitle);
            return -1;
        }
        char *data_type = getType(tblTitle, indCol);
        if(strcmp(data_type,"int") != 0){
            printf("Error: Data types incompatible\n");
            return -1;
        }
        unit.secondVal = atoi(returnValue(str1, indCol));
        return operandComparison(unit.firstVal, unit.secondVal, unit.operation);
    }
    else if(unit.str1 != NULL && unit.str2==NULL){
        if(unit.tableEntry2 != NULL && strcmp(unit.tableEntry2, tblTitle)!=0){
            printf("Error: Table %s exists instead of table %s\n", tblTitle, unit.tableEntry2);
            return -1;
        }
        int indCol = columnNumber(tblTitle, unit.columnEntry2);
        if (indCol == -1) {
            printf("Error: Column %s does not exist in table %s\n", unit.columnEntry2, tblTitle);
            return -1;
        }
        char *data_type = getType(tblTitle, indCol);
        if(strcmp(data_type,"str") != 0){
            printf("Error: Data types incompatible\n");
            return -1;
        }
        unit.str2 = returnValue(str1, indCol);
        return operandComparisonString(unit.str1, unit.str2, unit.operation);
    }
    else if(unit.str1 == NULL && unit.str2!=NULL){
        // printf("I'm %s %s here\n", tblTitle, unit.str2);
        if(unit.tableEntry1 != NULL && strcmp(unit.tableEntry1, tblTitle)!=0){
            printf("Error: Table %s exists instead of table %s\n", tblTitle, unit.tableEntry1);
            return -1;
        }
        int indCol = columnNumber(tblTitle, unit.columnEntry1);
        if (indCol == -1) {
            printf("Error: Column %s does not exist in table %s\n", unit.columnEntry1, tblTitle);
            return -1;
        }
        char *data_type = getType(tblTitle, indCol);
        if(strcmp(data_type,"str") != 0){
            printf("Error: Data types incompatible\n");
            return -1;
        }
        unit.str1 = returnValue(str1, indCol);
        return operandComparisonString(unit.str1, unit.str2, unit.operation);
    }
    else {
        if(unit.tableEntry2 != NULL && strcmp(unit.tableEntry2, tblTitle)!=0){
            printf("Error: Table %s exists instead of table %s\n", tblTitle, unit.tableEntry2);
            return -1;
        }
        if(unit.tableEntry1 != NULL && strcmp(unit.tableEntry1, tblTitle)!=0){
            printf("Error: Table %s exists instead of table %s\n", tblTitle, unit.tableEntry1);
            return -1;
        }
        int colIndex1 = columnNumber(tblTitle, unit.columnEntry1);
        int colIndex2 = columnNumber(tblTitle, unit.columnEntry2);
        if (colIndex1 == -1) {
            printf("Error: Column %s does not exist in table %s\n", unit.columnEntry1, tblTitle);
            return -1;
        }
        if (colIndex2 == -1) {
            printf("Error: Column %s does not exist in table %s\n", unit.columnEntry2, tblTitle);
            return -1;
        }
        if(strcmp(getType(tblTitle, colIndex1),getType(tblTitle, colIndex2)) != 0){
            printf("Error: Data types incompatible\n");
            return -1;
        }
        if(strcmp(getType(tblTitle, colIndex1), "str")){
            unit.str1 = returnValue(str1, colIndex1);
            unit.str2 = returnValue(str1, colIndex2);
            return operandComparisonString(unit.str1, unit.str2, unit.operation);
        }
        else{
            unit.firstVal = atoi(returnValue(str1, colIndex1));
            unit.secondVal = atoi(returnValue(str1, colIndex2));
            return operandComparison(unit.firstVal, unit.secondVal, unit.operation);
        }
    }
    return -1;    
}


int computingCondEquiJoin(struct OrList condition, char *str1, char *str2, char *tableEntry1, char *tableEntry2){
    AndList* tp = condition.head;
    int result = 0;
    
    while(tp!=NULL){
        AndEntry* tp2 = tp->head;
        int val = 1;
        while(tp2!=NULL){
            char str_copy1[2000];
            memset(str_copy1, 0, 2000);
            sprintf(str_copy1, "%s", str1);
            char str_copy2[2000];
            memset(str_copy2, 0, 2000);
            sprintf(str_copy2, "%s", str2);
            int ret;
            if(tp2->isCondition == 1){
                ret = computingCondEquiJoin(*(tp2->nestedCond), str_copy1, str_copy2, tableEntry1, tableEntry2);
            } else {
                ret = comparatorEquiJoin(*tp2, str_copy1, str_copy2, tableEntry1, tableEntry2);
            }
            if(ret == -1){
                return -1;
            }
            if(tp2->isCondition ==1 && tp2->isNegation){
                ret = ret==0?1:0; 
            }
            val = val & ret;
            tp2 = tp2->nextPtr;
        }
        if(val==1){
            return 1;
        }
        result = result | val;
        tp = tp->nextPtr;
    }
    return result;
}

int comparatorEquiJoin(struct AndEntry unit, char *str1, char *str2, char *tableEntry1, char *tableEntry2) {
    if (unit.findInteger2) { // col oper INT
        int indCol = columnNumber(unit.tableEntry1, unit.columnEntry1);
        if (indCol == -1) {
            printf("Error: Column %s does not exist in table %s\n", unit.columnEntry1, unit.tableEntry1);
            return -1;
        }
        if (strcmp(unit.tableEntry1, tableEntry1) == 0)
            unit.firstVal = atoi(returnValue(str1, indCol));
        else 
            unit.firstVal = atoi(returnValue(str2, indCol));
        return operandComparison(unit.firstVal, unit.secondVal, unit.operation);
    }
    else if (unit.str1 == NULL && unit.str2 != NULL) { // col oper str
        int indCol = columnNumber(unit.tableEntry1, unit.columnEntry1);
        if (indCol == -1) {
            printf("Error: Column %s does not exist in table %s\n", unit.columnEntry1, unit.tableEntry1);
            return -1;
        }
        if (strcmp(unit.tableEntry1, tableEntry1) == 0){
            unit.str1 = returnValue(str1, indCol);
        }
        else{
            unit.str1 = returnValue(str2, indCol);   
        }
        return operandComparisonString(unit.str1, unit.str2, unit.operation);
    }
    else { // col oper col
        int colIndex1 = columnNumber(unit.tableEntry1, unit.columnEntry1);
        int colIndex2 = columnNumber(unit.tableEntry2, unit.columnEntry2);
        if (colIndex1 == -1) {
            printf("Error: Column %s does not exist in table %s\n", unit.columnEntry1, unit.tableEntry1);
            return -1;
        }
        if (colIndex2 == -1) {
            printf("Error: Column %s does not exist in table %s\n", unit.columnEntry2, unit.tableEntry2);
            return -1;
        }
        char d1[50];
        sprintf(d1, "%s", getType(unit.tableEntry1, columnNumber(unit.tableEntry1, unit.columnEntry1)));
        if (strcmp(d1, "int") == 0) {
            if (strcmp(unit.tableEntry1, tableEntry1) == 0)
                unit.firstVal = atoi(returnValue(str1, colIndex1));
            else 
                unit.firstVal = atoi(returnValue(str2, colIndex1));

            if (strcmp(unit.tableEntry2, tableEntry2) == 0)
                unit.secondVal = atoi(returnValue(str2, colIndex2));
            else 
                unit.secondVal = atoi(returnValue(str1, colIndex2));

            return operandComparison(unit.firstVal, unit.secondVal, unit.operation);
        }
        else if (strcmp(d1, "str") == 0) {
            if (strcmp(unit.tableEntry1, tableEntry1) == 0)
                unit.str1 = returnValue(str1, colIndex1);
            else 
                unit.str1 = returnValue(str2, colIndex1);

            if (strcmp(unit.tableEntry2, tableEntry2) == 0)
                unit.str2 = returnValue(str2, colIndex2);
            else 
                unit.str2 = returnValue(str1, colIndex2);

            return operandComparisonString(unit.str1, unit.str2, unit.operation);
        }
    }
    return -1;    
}


int attachTable(char *tableEntry1, char *tableEntry2, struct OrList *conditions) {
    AndList *tp = conditions->head;
    while(tp != NULL) {
        AndEntry* tp2 = tp->head;
        while (tp2 != NULL) {
            if (tp2->isCondition == 1) {
                if (attachTable(tableEntry1, tableEntry2, tp2->nestedCond) == -1) return -1;
            }
            else {
                if (tp2->findInteger2) { // col oper INT
                    if (tp2->tableEntry1 == NULL) {
                        if (columnNumber(tableEntry1, tp2->columnEntry1) != -1 && columnNumber(tableEntry2, tp2->columnEntry1) != -1) {
                            fprintf(stdout, "Success at line no %d: Column %s exists in tables %s and %s\n", yylinenumber, tp2->columnEntry1, tableEntry1, tableEntry2);
                            return -1;
                        }
                        else if (columnNumber(tableEntry1, tp2->columnEntry1) != -1) tp2->tableEntry1 = tableEntry1;
                        else if (columnNumber(tableEntry2, tp2->columnEntry1) != -1) tp2->tableEntry1 = tableEntry2;
                        else {
                            fprintf(stdout, "Error at line no %d: Column %s does not exist in table %s or %s\n", yylinenumber, tp2->columnEntry1, tableEntry1, tableEntry2);
                            return -1;
                        }
                    }
                    else {
                        if (strcmp(tp2->tableEntry1, tableEntry1) != 0 && strcmp(tp2->tableEntry1, tableEntry2) != 0) {
                            fprintf(stdout, "Error at line no %d: Column %s does not exist in table %s or %s\n", yylinenumber,tp2->columnEntry1, tableEntry1, tableEntry2);
                            return -1;
                        }
                        else if (columnNumber(tp2->tableEntry1, tp2->columnEntry1) == -1) {
                            fprintf(stdout, "Error at line no %d: Error: Column %s does not exist in table %s\n",yylinenumber, tp2->columnEntry1, tp2->tableEntry1);
                            return -1;
                        }
                    }
                    char d1[50];
                    sprintf(d1, "%s", getType(tp2->tableEntry1, columnNumber(tp2->tableEntry1, tp2->columnEntry1)));
                    if (strcmp(d1, "int") != 0) {
                        fprintf(stdout, "Error at line no %d: Column %s is of type %s not INT\n\n", yylinenumber,tp2->columnEntry1, d1);
                        return -1;
                    }
                }
                else if (tp2->str2 != NULL) { // col oper str
                    if (columnNumber(tableEntry1, tp2->columnEntry1) != -1 && columnNumber(tableEntry2, tp2->columnEntry1) != -1) {
                        fprintf(stdout, "Success at line no %d: Column %s exists in tables %s and %s\n", yylinenumber, tp2->columnEntry1, tableEntry1, tableEntry2);
                        return -1;
                    }
                    if (tp2->tableEntry1 == NULL) {
                        if (columnNumber(tableEntry1, tp2->columnEntry1) != -1) tp2->tableEntry1 = tableEntry1;
                        else if (columnNumber(tableEntry2, tp2->columnEntry1) != -1) tp2->tableEntry1 = tableEntry2;
                        else {
                            fprintf(stdout, "Error at line no %d: Column %s does not exist in table %s or %s\n", yylinenumber,tp2->columnEntry1, tableEntry1, tableEntry2);
                            return -1;
                        }
                    }
                    else {
                        if (strcmp(tp2->tableEntry1, tableEntry1) != 0 && strcmp(tp2->tableEntry1, tableEntry2) != 0) {
                            fprintf(stdout, "Error at line no %d: Column %s does not exist in table %s or %s\n", yylinenumber,tp2->columnEntry1, tableEntry1, tableEntry2);
                            return -1;
                        }
                        else if (columnNumber(tp2->tableEntry1, tp2->columnEntry1) == -1) {
                            fprintf(stdout, "Error at line no %d: Error: Column %s does not exist in table %s\n", yylinenumber,tp2->columnEntry1, tp2->tableEntry1);
                            return -1;
                        }
                    }
                    char d1[50];
                    sprintf(d1, "%s", getType(tp2->tableEntry1, columnNumber(tp2->tableEntry1, tp2->columnEntry1)));
                    if (strcmp(d1, "str") != 0) {
                        fprintf(stdout, "Error at line no %d: Column %s is of type %s not STRING\n", yylinenumber,tp2->columnEntry1, d1);
                        return -1;
                    }
                }
                else { // col oper col
                    if (tp2->tableEntry1 == NULL) {
                        if (columnNumber(tableEntry1, tp2->columnEntry1) != -1 && columnNumber(tableEntry2, tp2->columnEntry1) != -1) {
                            fprintf(stdout, "Success at line no %d: Column %s exists in tables %s and %s\n", yylinenumber, tp2->columnEntry1, tableEntry1, tableEntry2);
                            return -1;
                        }
                        if (columnNumber(tableEntry1, tp2->columnEntry1) != -1) tp2->tableEntry1 = tableEntry1;
                        else if (columnNumber(tableEntry2, tp2->columnEntry1) != -1) tp2->tableEntry1 = tableEntry2;
                        else {
                            fprintf(stdout, "Error at line no %d: Column %s does not exist in table %s or %s\n", yylinenumber,tp2->columnEntry1, tableEntry1, tableEntry2);
                            return -1;
                        }
                    }
                    else {
                        if (strcmp(tp2->tableEntry1, tableEntry1) != 0 && strcmp(tp2->tableEntry1, tableEntry2) != 0) {
                            fprintf(stdout, "Error at line no %d: Column %s does not exist in table %s or %s\n", yylinenumber,tp2->columnEntry1, tableEntry1, tableEntry2);
                            return -1;
                        }
                        else if (columnNumber(tp2->tableEntry1, tp2->columnEntry1) == -1) {
                            fprintf(stdout, "Error at line no %d: Error: Column %s does not exist in table %s\n", yylinenumber,tp2->columnEntry1, tp2->tableEntry1);
                            return -1;
                        }
                    }
                    if (tp2->tableEntry2 == NULL) {
                        if (columnNumber(tableEntry1, tp2->columnEntry1) != -1 && columnNumber(tableEntry2, tp2->columnEntry1) != -1) {
                            fprintf(stdout, "Success at line no %d: Column %s exists in tables %s and %s\n", yylinenumber, tp2->columnEntry1, tableEntry1, tableEntry2);
                            return -1;
                        }
                        if (columnNumber(tableEntry1, tp2->columnEntry2) != -1) tp2->tableEntry2 = tableEntry1;
                        else if (columnNumber(tableEntry2, tp2->columnEntry2) != -1) tp2->tableEntry2 = tableEntry2;
                        else {
                            fprintf(stdout, "Error at line no %d: Column %s does not exist in table %s or %s\n", yylinenumber,tp2->columnEntry2, tableEntry1, tableEntry2);
                            return -1;
                        }
                    }
                    else {
                        if (strcmp(tp2->tableEntry2, tableEntry1)!=0 && strcmp(tp2->tableEntry2, tableEntry2) != 0) {
                            fprintf(stdout, "Error at line no %d: Column %s does not exist in table %s or %s\n", yylinenumber,tp2->columnEntry2, tableEntry1, tableEntry2);
                            return -1;
                        }
                        else if (columnNumber(tp2->tableEntry2, tp2->columnEntry2) == -1) {
                            fprintf(stdout, "Error at line no %d: Error: Column %s does not exist in table %s\n", yylinenumber,tp2->columnEntry2, tp2->tableEntry2);
                            return -1;
                        }
                    }
                    char d1[50], d2[50];
                    sprintf(d1, "%s", getType(tp2->tableEntry1, columnNumber(tp2->tableEntry1, tp2->columnEntry1)));
                    sprintf(d2, "%s", getType(tp2->tableEntry2, columnNumber(tp2->tableEntry2, tp2->columnEntry2)));
                    if (strcmp(d1, d2) != 0) {
                        fprintf(stdout, "Error at line no %d: Column %s and %s do not have same data types\n", yylinenumber,tp2->columnEntry1, tp2->columnEntry2);
                        return -1;
                    }
                }
                
            }
            tp2 = tp2->nextPtr;
        }
        tp = tp->nextPtr;
    }
    return 1;
}

bool is_table_present(char *table){
    FILE * f=fopen("testfile/tablenames.txt","r");
    char text[max_length];
    const char *deli=",";
    char *token;
    fgets(text,max_length,f);
    sscanf(text,"%[^\n]s",text);
    token=strtok(text,deli);
    char match_table[200];
    memset(match_table,0,sizeof(match_table));
    sprintf(match_table,"%s.csv",table);

    while(token!=NULL){
        if(strcmp(token,match_table)==0) return true;
        token=strtok(NULL,deli);
    }

    return false;

}

void cartesian_product(char * tableEntry1,char * tableEntry2){
    bool is_pres1=is_table_present(tableEntry1);
    bool is_pres2=is_table_present(tableEntry2);
    if(!is_pres1){
        fprintf(stderr,"Table %s is not present\n",tableEntry1);
        return;
    }
    if(!is_pres2){
        fprintf(stderr,"Table %s is not present\n",tableEntry2);
        return;
    }

    char file_name[2][200];
    memset(file_name,0,sizeof(file_name));
    sprintf(file_name[0],"testfile/%s.csv",tableEntry1);
    sprintf(file_name[1],"testfile/%s.csv",tableEntry2);

    FILE *f1=fopen(file_name[0],"r");
    FILE *f2=fopen(file_name[1],"r");

    char text1[max_length];
    fgets(text1,max_length,f1);
    sscanf(text1,"%[^\n]s",text1);
    char *token;
    const char *deli=",";
    token=strtok(text1,deli);
    while(token!=NULL){
        printf("%s.%s, ",tableEntry1,token);
        token=strtok(NULL,deli);
    }
    char text2[max_length];
    fgets(text2,max_length,f2);
    // sscanf(text1,"%[^\n]s",text1);
    token=strtok(text2,deli);
    while(token!=NULL){
        printf("%s.%s",tableEntry2,token);
        token=strtok(NULL,deli);
        if(token!=NULL) printf(", ");
    }
    fgets(text1,max_length,f1);
    fgets(text2,max_length,f2);
    fclose(f2);
    int record_count=0;
    
    while(fgets(text1,max_length,f1)){
        sscanf(text1,"%[^\n]s",text1);
        f2=fopen(file_name[1],"r");
        int i;
        for(i=0;i<2;i++) fgets(text2,max_length,f2);

        while(fgets(text2,max_length,f2)){
            printf("%s,%s",text1,text2);
            record_count++;
        }
        fclose(f2);
    }

    printf("The number of records found: %d\n",record_count);
    fclose(f1);
}

void projection(char cols_name[200][200], int total_cols, char *table) {
    char file_name[200];
    memset(file_name,0,sizeof(file_name));
    sprintf(file_name,"testfile/%s.csv",table);

    FILE *fp = fopen(file_name, "r");
    bool print[200];
    memset(print, 0, sizeof(print));

    char headings[max_length];
    fgets(headings, max_length, fp);
    sscanf(headings, "%[^\n]s", headings);

    char headings_copy[max_length];
    strcpy(headings_copy, headings);

    const char *deli = ",";
    char *token;

    int cols_count = 0;
    token = strtok(headings, deli);
    int col_index = 0;

    while (token) {
        for(int i = 0; i < total_cols; i++) {
            if( !strcmp(token, cols_name[i]) ) {
                print[col_index] = true;
                cols_count++;
            }
        }
        token = strtok(NULL, deli);
        col_index++;
    }

    if(cols_count == total_cols) {
        token = strtok(headings_copy, deli);
        int col_index = 0;
        while (token) {
            if(print[col_index] == true) printf("%s ", token);
            token = strtok(NULL, deli);
            col_index++;
        }
        printf("\n");

        int record_count = 0;
        char text[max_length];
        fgets(text, max_length, fp);

        while (fgets(text, max_length, fp)) {
            sscanf(text, "%[^\n]s", text);
            char *token;
            token = strtok(text, deli);

            int col_index = 0;
            while (token) {
                if(print[col_index] == true) printf("%s ", token);
                token = strtok(NULL, deli);
                col_index++;
            }
            printf("\n");
            record_count++;
        }

        printf("The number of records found: %d\n", record_count);

    }
    else {
        printf("ERROR: %d column(s) not found.\n", total_cols-cols_count);
    }

    fclose(fp);
}
