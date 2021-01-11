#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int execmd(char *cmd, char *result)
{
    strcat(cmd, " 2>1.txt"); //将错误内容输出到1.txt
    if (system(cmd))
        return 0; //返回0表示运行失败
    FILE *p;
    if ((p = fopen("1.txt", "r+")) != NULL) //存在文件1.txt
    {
        fgets(result, 128, p); //将错误传给result
        fclose(p);
        system("del 1.txt"); //删除1.txt
        if (strcmp(result, "") == 0)
            return 1; //返回1表示运行成功
        else
            return 0;
    }
    else
        return 1; //不存在1.txt 默认执行成功
}

void CreateDataBase(char *name)
{
    //printf("create database %s\n", name);
    char ndbf[20]; //新数据库文件名
    strcpy(ndbf, name);
    strcat(ndbf, ".dbf");
    FILE *p;
    if ((p = fopen(ndbf, "rb")) != NULL) //该数据库已存在
    {
        printf("Database already exists!\n");
        printf("Create database %s failed!\n", name);
        //exit(1);
    }
    else if ((p = fopen(ndbf, "ab+")) == NULL) //未成功创建
    {
        printf("Create file error!\n");
        printf("Create database %s failed!\n", name);
        //exit(1);
    }
    else //创建成功
        printf("Create database %s successfully!\n", name);
    fclose(p);
}

void OpenDataBase(char *name)
{
    //printf("open database %s\n", name);
    char ndbf[20]; //新数据库文件名
    strcpy(ndbf, name);
    strcat(ndbf, ".dbf");
    if (strcmp(ndbf, dbf) == 0) //该数据库已打开
    {
        printf("Database already opens!\n");
        return;
    }
    if (fp != NULL || dopens != 0) //有打开的数据库
    {
        printf("Please close the current database!\n");
        return;
    }
    if ((fp = fopen(ndbf, "rb+")) == NULL) //数据库不存在或文件打开失败
    {
        printf("No such database!\n");
        //exit(1);
    }
    else //打开成功
    {
        printf("Open database %s successfully!\n", name);
        strcpy(dbf, ndbf);
        dopens = 1;
    }
}

void CloseDataBase(char *name)
{
    //printf("close database %s\n", name);
    if (dopens == 0) //当前没有打开的数据库
    {
        printf("No database is open!\n");
        printf("Close %s failed!\n", name);
        return;
    }
    char ndbf[20]; //需要关闭的数据库文件名
    strcpy(ndbf, name);
    strcat(ndbf, ".dbf");
    if (strcmp(ndbf, dbf) != 0) //需要关闭的数据库名和当前打开的数据库名不同
    {
        printf("The current database is not %s!\n", name);
        return;
    }
    else if (fclose(fp) == 0) //关闭成功
    {
        printf("Close %s successfully!\n", name);
        strcpy(dbf, "");
        fclose(fp);
        dopens = 0;
    }
    else //关闭失败
        printf("Close database %s failed!\n", name);
}

void DropDataBase(char *name)
{
    //printf("drop database %s\n", name);
    char ndbf[20];
    strcpy(ndbf, name);
    strcat(ndbf, ".dbf");
    if (strcmp(ndbf, dbf) == 0) //数据库未关闭
    {
        printf("Please close database %s first!\n", name);
        return;
    }
    char command[30]; //cmd指令
    strcpy(command, "del ");
    strcat(command, ndbf);
    char result[1024] = "";
    if (1 == execmd(command, result)) //执行命令，若执行成功返回1
    {
        printf("drop database %s successfully!\n", name);
    }
    else //执行失败输出错误信息
    {
        printf(result);
        printf("drop database %s failed!\n", name);
    }
}

void ViewDataBase(char *name)
{
    //printf("view database %s\n", name);
    char tempdbf[20]; //临时dbf文件名
    strcpy(tempdbf, name);
    strcat(tempdbf, ".dbf");
    FILE *p;
    if ((p = fopen(tempdbf, "rb")) == NULL) //打开失败，没有该数据库
        printf("No such database!\n");
    int haveTable=0;
    while (!feof(p))
    {
        int num;
        TableMode FieldSet[MAX_SIZE];
        char tempc;
        int i = fread(&tempc, sizeof(char), 1, p);
        if (!i) //读取失败，返回
            return;
        if (tempc != '~') //文件格式有问题，返回
        {
            printf("%s format not correct!\n", tempdbf);
            fclose(p);
            return;
        }
        if(!haveTable){ //有表则输出
            haveTable=1;
            printf("There are tables in database %s:\n", name);
        }
        char tname[20];
        fread(tname, sizeof(char), FILE_NAME_LENGTH, p); //读取表格信息
        fread(&num, sizeof(int), 1, p);
        fread(FieldSet, sizeof(TableMode), num, p);
        printf("%s\n", tname);
    }
    fclose(p);
}

void CreateTable(char *name)
{
    //printf("create table %s\n", name);
    //fp = fopen("data.dbf", "ab+");
    if (fp == NULL||dopens==0) //当前未打开数据库
        printf("No database is open!\nPlease open database first!\n");
    fseek(fp, 0L, SEEK_END); //移动指针到文件尾
    char temp[10];
    scanf("%s", temp);
    if (strcmp(temp, "(") == 0) //(
    {
        TableMode FieldSet[MAX_SIZE];
        int num = 0;
        while (1)
        {
            scanf("%15s%8s%d", FieldSet[num].sFieldName, FieldSet[num].sType, &(FieldSet[num].iSize)); //读取字段名，字段类型，字段字长
            char tempKey[10];
            scanf("%s", tempKey); //读取字段是否为主键
            FieldSet[num].bKey = tempKey[0];
            //printf("%s%s%d%c", FieldSet[num].sFieldName, FieldSet[num].sType, FieldSet[num].iSize, FieldSet[num].bKey);
            char tempNull[10];
            scanf("%s", tempNull); //读取字段是否为空
            FieldSet[num].bNullFlag = tempNull[0];
            FieldSet[num].bValidFlag = 'y'; //默认字段有效
            if (FieldSet[num].bKey != 'n' && FieldSet[num].bKey != 'y') //输入bKey不合法
            {
                printf("命令语句有误!\n");
                return;
            }
            else if (FieldSet[num].bNullFlag != 'n' && FieldSet[num].bNullFlag != 'y') //输入bNullFlag不合法
            {
                printf("命令语句有误!\n");
                return;
            }
            num++;
            if (tempNull[1] == ',') //如果有逗号说明还有下一个字段，否则跳出循环
                continue;
            else
                break;
        }
        scanf("%s", temp);
        if (strcmp(temp, ")") == 0) //) 格式正确，存入表信息
        {
            fwrite("~", sizeof(char), 1, fp);
            fwrite(name, sizeof(char), FILE_NAME_LENGTH, fp);
            fwrite(&num, sizeof(int), 1, fp);
            fwrite(FieldSet, sizeof(TableMode), num, fp);
            fseek(fp,0L,SEEK_SET);
            printf("create table %s successfully!\n",name);
        }
        else
            printf("命令语句有误!\n");
    }
    else
        printf("命令语句有误!\n");
}

void DropTable(char *name)
{
    printf("drop table %s\n", name);
}

int OpenTable(char *name, PTableMode FieldSet)
{
    //printf("open table %s\n", name);
    //fp = fopen("data.dbf", "rb+");
    if (fp == NULL) //未打开数据库
        printf("No database is open!\nPlease open database first!\n");
    rewind(fp); //移动到文件开始
    while (!feof(fp))
    {
        char tempc;
        int i = fread(&tempc, sizeof(char), 1, fp);
        if (tempc != '~' || !i) //格式不对
        {
            printf("%s format not correct!\n", dbf);
            return -1; //-1表示没找到
        }
        char tempName[20];
        int num = 0;
        fread(tempName, sizeof(char), FILE_NAME_LENGTH, fp); //读取表名
        fread(&num, sizeof(int), 1, fp); //读取字段数
        fread(FieldSet, sizeof(TableMode), num, fp); //读取结构
        if (strcmp(tempName, name) == 0) //找到了表
        {
            topens=1;
            return num; //返回字段个数
        }
    }
    return -1;
}

void ViewTable(char *name)
{
    //printf("view table %s\n", name);
    TableMode FieldSet[MAX_SIZE];
    int num = OpenTable(name, FieldSet);
    if (num == -1) //-1表示没找到表
        printf("No such table!\n");
    else if (num == 0) //0表示没有字段
        printf("No field in table!\n");
    else
    {
        printf("FieldName      Type    Size Key  Null\n");
        for (int i = 0; i < num; i++)
            printf("%-15s%-8s%-5d%c    %c\n", FieldSet[i].sFieldName, FieldSet[i].sType,
                   FieldSet[i].iSize, FieldSet[i].bKey, FieldSet[i].bNullFlag);
    }
}
