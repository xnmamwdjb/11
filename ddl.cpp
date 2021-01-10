#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int execmd(char *cmd, char *result)
{
    strcat(cmd," 2>1.txt");
    if (system(cmd))
        return 0; //返回0表示运行失败
    FILE *p;
    if ((p = fopen("1.txt", "r+")) != NULL)
    {
        fgets(result, 128, p);
        fclose(p);
        system("del 1.txt");
        if (strcmp(result, "") == 0)
            return 1;
        else
            return 0;
    }
    return 1; //返回1表示运行成功
}

void CreateDataBase(char *name)
{
    //printf("create database %s\n", name);
    strcpy(dbf, name);
    strcat(dbf, ".dbf");
    if (fp != NULL)
        fclose(fp);
    if ((fp = fopen(dbf, "rb+")) != NULL) //数据库已存在
    {
        printf("Database already exists!\n");
        printf("Create database %s failed!\n", name);
        return;
        //exit(1);
    }
    if ((fp = fopen(dbf, "ab+")) == NULL) //未成功创建
    {
        printf("Create file error!\n");
        printf("Create database %s failed!\n", name);
        strcpy(dbf, "");
        //exit(1);
    }
    else //创建成功
        printf("Create database %s successfully!\n", name);
}

void OpenDataBase(char *name)
{
    //printf("open database %s\n", name);
    char ndbf[20];
    strcpy(ndbf, name);
    strcat(ndbf, ".dbf");
    strcpy(dbf, name);
    strcat(dbf, ".dbf");
    if (strcmp(ndbf, dbf) == 0) //数据库已打开
    {
        printf("Database already opens!\n");
        return;
    }
    if (fp != NULL)
        fclose(fp);
    if ((fp = fopen(dbf, "rb+")) == NULL) //打开文件失败
    {
        printf("Open file error!\n");
        strcpy(dbf, "");
        //exit(1);
    }
    else //打开成功
        printf("Open database %s successfully!\n", name);
}

void CloseDataBase(char *name)
{
    //printf("close database %s\n", name);
    if (strcmp(dbf, "") == 0) //当前没有打开的数据库
    {
        printf("No database is open!\n");
        printf("Close %s failed!\n", name);
        return;
    }
    strcpy(dbf, name);
    strcat(dbf, ".dbf");
    if (fclose(fp) == 0) //关闭成功
    {
        printf("Close %s successfully!\n", name);
        strcpy(dbf, "");
    }
    else //关闭失败
        printf("Close database %s failed!\n", name);
}

void DropDataBase(char *name)
{
    //printf("drop database %s\n", name);
    char command[30];
    strcpy(command, "del ");
    strcat(command, name);
    strcat(command, ".dbf");
    char result[1024] = "";
    if (1 == execmd(command, result)) //执行命令，若执行成功返回1
    {
        printf("drop database %s successfully!\n", name);
    }
    else
    {
        printf(result);
        printf("drop database %s failed!\n", name);
    }
}

void CreateTable(char *name)
{
    printf("create table %s\n", name);
}

void DropTable(char *name)
{
    printf("drop table %s\n", name);
}

void OpenTable(char *name)
{
    printf("open table %s\n", name);
}
