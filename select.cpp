#include<stdio.h>
#include<string.h>
void Select(char* colName, char* tableName, char* whereColName, char* value){
    printf("select %s from %s where %s = %s\n",colName, tableName, whereColName, value);
}