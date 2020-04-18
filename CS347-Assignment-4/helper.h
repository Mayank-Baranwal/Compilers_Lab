#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern int yylinenumber;

typedef struct AndEntry{
    int operation;
    int firstVal;
    int secondVal;
    int findInteger1, findInteger2;
    int isCondition;
    int isNegation;
    char *tableEntry1, *tableEntry2;
    char *columnEntry1, *columnEntry2;
    char *str1, *str2;
    struct AndEntry* nextPtr;
    struct OrList* nestedCond;
} AndEntry;

typedef struct AndList {
    AndEntry* head;
    AndEntry* tail;
    struct AndList* nextPtr;
} AndList;

typedef struct OrList {
    AndList* head;
    AndList* tail;
} OrList;

AndList combineAndList(struct AndList and_condition, struct AndEntry expr);
OrList combineOrList(struct OrList or_condition, struct AndList and_condition);
void ShowList(struct OrList or_condition);
char *getType(char *tbl, int indCol);
char *returnValue(char *str, int indCol);
int columnNumber(char *tbl, char *col);
int showEquiJoin(char *tableEntry1, char *tableEntry2, struct OrList *conditions);
int computingCondSelect(struct OrList condition, char *str, char* tblTitle);
int makeComplement(int oper);
int operandComparison(int num1, int num2, int oper);
int operandComparisonString(char *str1, char *str2, int oper);
int comparatorSelect(struct AndEntry unit, char *str1, char* tblTitle);
int computingCondEquiJoin(struct OrList condition, char *str1, char *str2, char *tableEntry1, char *tableEntry2);
int comparatorEquiJoin(struct AndEntry unit, char *str1, char *str2, char *tableEntry1, char *tableEntry2);
int attachTable(char *tableEntry1, char *tableEntry2, struct OrList *conditions);
bool is_table_present(char *table);
void cartesian_product(char * tableEntry1,char * tableEntry2);
void projection(char cols_name[200][200], int total_cols, char *table);