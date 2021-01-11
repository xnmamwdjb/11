#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int execmd(char *cmd, char *result)
{
    strcat(cmd, " 2>1.txt"); //���������������1.txt
    if (system(cmd))
        return 0; //����0��ʾ����ʧ��
    FILE *p;
    if ((p = fopen("1.txt", "r+")) != NULL) //�����ļ�1.txt
    {
        fgets(result, 128, p); //�����󴫸�result
        fclose(p);
        system("del 1.txt"); //ɾ��1.txt
        if (strcmp(result, "") == 0)
            return 1; //����1��ʾ���гɹ�
        else
            return 0;
    }
    else
        return 1; //������1.txt Ĭ��ִ�гɹ�
}

void CreateDataBase(char *name)
{
    //printf("create database %s\n", name);
    char ndbf[20]; //�����ݿ��ļ���
    strcpy(ndbf, name);
    strcat(ndbf, ".dbf");
    FILE *p;
    if ((p = fopen(ndbf, "rb")) != NULL) //�����ݿ��Ѵ���
    {
        printf("Database already exists!\n");
        printf("Create database %s failed!\n", name);
        //exit(1);
    }
    else if ((p = fopen(ndbf, "ab+")) == NULL) //δ�ɹ�����
    {
        printf("Create file error!\n");
        printf("Create database %s failed!\n", name);
        //exit(1);
    }
    else //�����ɹ�
        printf("Create database %s successfully!\n", name);
    fclose(p);
}

void OpenDataBase(char *name)
{
    //printf("open database %s\n", name);
    char ndbf[20]; //�����ݿ��ļ���
    strcpy(ndbf, name);
    strcat(ndbf, ".dbf");
    if (strcmp(ndbf, dbf) == 0) //�����ݿ��Ѵ�
    {
        printf("Database already opens!\n");
        return;
    }
    if (fp != NULL || dopens != 0) //�д򿪵����ݿ�
    {
        printf("Please close the current database!\n");
        return;
    }
    if ((fp = fopen(ndbf, "rb+")) == NULL) //���ݿⲻ���ڻ��ļ���ʧ��
    {
        printf("No such database!\n");
        //exit(1);
    }
    else //�򿪳ɹ�
    {
        printf("Open database %s successfully!\n", name);
        strcpy(dbf, ndbf);
        dopens = 1;
    }
}

void CloseDataBase(char *name)
{
    //printf("close database %s\n", name);
    if (dopens == 0) //��ǰû�д򿪵����ݿ�
    {
        printf("No database is open!\n");
        printf("Close %s failed!\n", name);
        return;
    }
    char ndbf[20]; //��Ҫ�رյ����ݿ��ļ���
    strcpy(ndbf, name);
    strcat(ndbf, ".dbf");
    if (strcmp(ndbf, dbf) != 0) //��Ҫ�رյ����ݿ����͵�ǰ�򿪵����ݿ�����ͬ
    {
        printf("The current database is not %s!\n", name);
        return;
    }
    else if (fclose(fp) == 0) //�رճɹ�
    {
        printf("Close %s successfully!\n", name);
        strcpy(dbf, "");
        dopens = 0;
    }
    else //�ر�ʧ��
        printf("Close database %s failed!\n", name);
}

void DropDataBase(char *name)
{
    //printf("drop database %s\n", name);
    char ndbf[20];
    strcpy(ndbf, name);
    strcat(ndbf, ".dbf");
    if (strcmp(ndbf, dbf) == 0)
    {
        printf("Please close database %s first!\n", name);
        return;
    }
    char command[30];
    strcpy(command, "del ");
    strcat(command, ndbf);
    char result[1024] = "";
    if (1 == execmd(command, result)) //ִ�������ִ�гɹ�����1
    {
        printf("drop database %s successfully!\n", name);
    }
    else
    {
        printf(result);
        printf("drop database %s failed!\n", name);
    }
}

void ViewDataBase(char *name)
{
    //printf("view database %s\n", name);
    char tempdbf[20];
    strcpy(tempdbf, name);
    strcat(tempdbf, ".dbf");
    FILE *p;
    if ((p = fopen(tempdbf, "rb")) == NULL)
        printf("No such database!\n");
    printf("There are tables in database %s:\n", name);
    while (!feof(p))
    {
        int num;
        TableMode FieldSet[MAX_SIZE];
        char tempc;
        int i = fread(&tempc, sizeof(char), 1, p);
        if (!i)
            return;
        if (tempc != '~')
        {
            printf("%s format not correct!\n", tempdbf);
            fclose(p);
            return;
        }
        char tname[20];
        fread(tname, sizeof(char), FILE_NAME_LENGTH, p);
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
    if (fp == NULL)
        printf("No database is open!\nPlease open database first!\n");
    fseek(fp, 0L, SEEK_END);
    char temp[10];
    scanf("%s", temp);
    if (strcmp(temp, "(") == 0)
    {
        TableMode FieldSet[MAX_SIZE];
        int num = 0;
        while (1)
        {
            scanf("%15s%8s%d", FieldSet[num].sFieldName, FieldSet[num].sType, &(FieldSet[num].iSize));
            char tempKey[10];
            scanf("%s", tempKey);
            FieldSet[num].bKey = tempKey[0];
            printf("%s%s%d%c", FieldSet[num].sFieldName, FieldSet[num].sType, FieldSet[num].iSize, FieldSet[num].bKey);
            char tempNull[10];
            scanf("%s", tempNull);
            FieldSet[num].bNullFlag = tempNull[0];
            FieldSet[num].bValidFlag = 'y';
            if (FieldSet[num].bKey != 'n' && FieldSet[num].bKey != 'y')
            {
                printf("�����������!\n");
                return;
            }
            else if (FieldSet[num].bNullFlag != 'n' && FieldSet[num].bNullFlag != 'y')
            {
                printf("�����������!\n");
                return;
            }
            num++;
            if (tempNull[1] == ',')
                continue;
            else
                break;
        }
        scanf("%s", temp);
        if (strcmp(temp, ")") == 0)
        {
            fwrite("~", sizeof(char), 1, fp);
            fwrite(name, sizeof(char), FILE_NAME_LENGTH, fp);
            fwrite(&num, sizeof(int), 1, fp);
            fwrite(FieldSet, sizeof(TableMode), num, fp);
        }
        else
            printf("�����������!\n");
    }
    else
        printf("�����������!\n");
}

void DropTable(char *name)
{
    printf("drop table %s\n", name);
}

int OpenTable(char *name, PTableMode FieldSet)
{
    //printf("open table %s\n", name);
    //fp = fopen("data.dbf", "rb+");
    if (fp == NULL)
        printf("No database is open!\nPlease open database first!\n");
    rewind(fp);
    while (!feof(fp))
    {
        char tempc;
        int i = fread(&tempc, sizeof(char), 1, fp);
        if (tempc != '~' || !i)
        {
            printf("%s format not correct!\n", dbf);
            return -1;
        }
        char tempName[20];
        int num = 0;
        fread(tempName, sizeof(char), FILE_NAME_LENGTH, fp);
        fread(&num, sizeof(int), 1, fp);
        fread(FieldSet, sizeof(TableMode), num, fp);
        if (strcmp(tempName, name) == 0)
        {
            topens=1;
            return num;
        }
    }
    return -1;
}

void ViewTable(char *name)
{
    //printf("view table %s\n", name);
    TableMode FieldSet[MAX_SIZE];
    int num = OpenTable(name, FieldSet);
    if (num == -1)
        printf("No such table!\n");
    else if (num == 0)
        printf("No field in table!\n");
    else
    {
        printf("FieldName      Type    Size Key  Null\n");
        for (int i = 0; i < num; i++)
            printf("%-15s%-8s%-5d%c    %c\n", FieldSet[i].sFieldName, FieldSet[i].sType,
                   FieldSet[i].iSize, FieldSet[i].bKey, FieldSet[i].bNullFlag);
    }
}
