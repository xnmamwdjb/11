#include<stdio.h>
#include<string.h>

void Insert(char* name,char* value){
    printf("insert into %s %s\n",name,value);
}
void Delete(char* tableName,char* colName,char* value){
    printf("delete from %s where %s = %s\n",tableName,colName,value);
}
void update(char* tableName,char* newColName,char* newValue,char* colName,char* value){
    printf("update %s = %s from %s where %s = %s\n",tableName,newColName,newValue,colName,value);
}