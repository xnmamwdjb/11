#include<stdio.h>
#include<string.h>

void CreateDataBase(char* name){
    printf("create database %s\n",name);
}

void CreateTable(char* name){
    printf("create table %s\n",name);
}

void DropDataBase(char* name){
    printf("drop database %s\n",name);
}

void DropTable(char* name){
    printf("drop table %s\n",name);
}

void OpenDataBase(char* name){
	printf("open database %s\n",name);
}

void OpenTable(char* name){
	printf("open table %s\n",name);
}

void CloseDataBase(char* name){
	printf("close %s\n",name);
}