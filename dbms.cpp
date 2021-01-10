#include <stdio.h>
#include <string.h>

FILE *fp;
char dbf[20];

#define MAX_SIZE 50          //���������ֶ���󳤶�
#define FILE_NAME_LENGTH 15  //�ļ�����󳤶�
#define FIELD_NAME_LENGTH 15 //�ֶ�����󳤶�

typedef struct
{
    char sFieldName[FIELD_NAME_LENGTH]; //�ֶ���
    char sType[8]; //�ֶ�����
    int iSize; //�ֶ��ֳ�
    char bKey; //�ֶ��Ƿ���KEY��
    char bNullFlag; //�ֶ��Ƿ�����Ϊ��
    char bValidFlag; //�ֶ��Ƿ���Ч
} TableMode, *PTableMode; //�ֶνṹ

#include "select.cpp"
#include "dml.cpp"
#include "ddl.cpp"

void transfer(char *cmd)
{
    for (int i = 0; i < strlen(cmd); i++)
        cmd[i] = (cmd[i] >= 'A' && cmd[i] <= 'Z') ? cmd[i] + ('a' - 'A') : cmd[i];
}

int main()
{
    printf("�밴���¹��������������(����ִ�Сд)\n");
    printf("�½����ݿ�: create database ���ݿ���\n");
    printf("ɾ�����ݿ�: drop database ���ݿ���\n");
    printf("�����ݿ�: open database ���ݿ���\n");
    printf("�ر����ݿ�: close database ���ݿ���\n");
    printf("����±�  : create table  ����\n");
    printf("            (\n");
    printf("            �ֶ��� �������� �ֳ� �Ƿ�ΪKEY����y/n�� �Ƿ�ɿգ�y/n��,\n");
    printf("            �ֶ��� �������� �ֳ� �Ƿ�ΪKEY����y/n�� �Ƿ�ɿգ�y/n��,\n");
    printf("            ��          ��\n");
    printf("            )\n");
    printf("�鿴��ṹ: open table ����\n");
    printf("ɾ���ɱ�  : drop table ����\n");
    printf("��ѯ      : select ����(all��ʾ������) from ���� where ���� = ֵ(ֵ��Ϊall,��ȫ����Χ)\n");
    printf("����      : insert into ����(field1,field2,��) values(value1,value2,��)\n");
    printf("�޸�      : update ���� = ��ֵ from ���� where ���� = ֵ(ֵ��Ϊall,��ȫ����Χ)\n");
    printf("ɾ��      : delete from ���� where ���� = ֵ(ֵ��Ϊall,��ȫ����Χ)\n");
    printf("�˳�����  : exit\n");
    char cmd[10];
    while (~scanf("%s", cmd))
    {
        transfer(cmd);
        if (strcmp(cmd, "exit") == 0) //exit
            break;
        if (strcmp(cmd, "create") == 0) //create
        {
            //create database ���ݿ���
            //create table ����
            char name[20];
            scanf("%s", cmd);
            transfer(cmd);
            if (strcmp(cmd, "database") == 0) //database
            {
                scanf("%s", name);
                CreateDataBase(name);
            }
            else if (strcmp(cmd, "table") == 0) //table
            {
                scanf("%s", name);
                CreateTable(name);
            }
            else
                printf("�����������!\n");
        }
        else if (strcmp(cmd, "drop") == 0) //drop
        {
            //drop database ���ݿ���
            //drop table ����
            char name[20];
            scanf("%s", cmd);
            transfer(cmd);
            if (strcmp(cmd, "database") == 0) //database
            {
                scanf("%s", name);
                DropDataBase(name);
            }
            else if (strcmp(cmd, "table") == 0) //table
            {
                scanf("%s", name);
                DropTable(name);
            }
            else
                printf("�����������!\n");
        }
        else if (strcmp(cmd, "open") == 0) //open
        {
            //open database ���ݿ���
            //open table ����
            char name[20];
            scanf("%s", cmd);
            transfer(cmd);
            if (strcmp(cmd, "database") == 0) //database
            {
                scanf("%s", name);
                OpenDataBase(name);
            }
            else if (strcmp(cmd, "table") == 0) //table
            {
                scanf("%s", name);
                OpenTable(name);
            }
            else
                printf("�����������!\n");
        }
        else if (strcmp(cmd, "close") == 0) //close
        {
            //close ���ݿ���
            char name[20];
            scanf("%s", cmd);
            transfer(cmd);
            if (strcmp(cmd, "database") == 0) //database
            {
                scanf("%s", name);
                CloseDataBase(name);
            }
            else
                printf("�����������!\n");
        }
        else if (strcmp(cmd, "insert") == 0) //insert
        {
            //insert into ����(field1,field2,��) values(value1,value2,��)"
            char value[100], name[100];
            scanf("%s", cmd);
            transfer(cmd);
            if (strcmp(cmd, "into") == 0) //into
            {
                scanf("%s%s", name, value);
                Insert(name, value);
            }
            else
                printf("�����������!\n");
        }
        else if (strcmp(cmd, "delete") == 0) //delete
        {
            //delete from ���� where ���� = ֵ(ֵ��Ϊall, ��ȫ����Χ)
            char tableName[20], colName[20], equal[2], value[100];
            scanf("%s", cmd);
            transfer(cmd);
            if (strcmp(cmd, "from") == 0) //from
            {
                scanf("%s", tableName);
                scanf("%s", cmd);
                transfer(cmd);
                if (strcmp(cmd, "where") == 0) //where
                {
                    scanf("%s", colName);
                    scanf("%s", equal);
                    if (strcmp(equal, "=") == 0) //=
                    {
                        scanf("%s", value);
                        Delete(tableName, colName, value);
                    }
                    else
                        printf("�����������!\n");
                }
                else
                    printf("�����������!\n");
            }
            else
                printf("�����������!\n");
        }
        else if (strcmp(cmd, "update")==0) //update
        {
            //update ���� = ��ֵ from ���� where ���� = ֵ(ֵ��Ϊall, ��ȫ����Χ)
            char newColName[20], equal1[5], newValue[20], tableName[20], colName[20], equal2[5], value[20];
            scanf("%s", newColName);
            scanf("%s", equal1);
            if (strcmp(equal1, "=") == 0) //=
            {
                scanf("%s", newValue);
                scanf("%s", cmd);
                transfer(cmd);
                if (strcmp(cmd, "from") == 0) //from
                {
                    scanf("%s", tableName);
                    scanf("%s", cmd);
                    transfer(cmd);
                    if (strcmp(cmd, "where") == 0) //where
                    {
                        scanf("%s", colName);
                        scanf("%s", equal2);
                        if (strcmp(equal2, "=") == 0) //=
                        {
                            scanf("%s", value);
                            update(newColName, newValue, tableName, colName, value);
                        }
                        else
                            printf("�����������1!\n");
                    }
                    else
                        printf("�����������2!\n");
                }
                else
                    printf("�����������3!\n");
            }
            else
                printf("�����������4!\n");
        }
        else if (strcmp(cmd, "select")==0) //select
        {
            //select ����(all��ʾ������)  from ���� where ���� = ֵ(ֵ��Ϊall,��ȫ����Χ)"
            char colName[20], tableName[20], whereColName[20], equal[2], value[20];
            scanf("%s", colName);
            scanf("%s", cmd);
            transfer(cmd);
            if (strcmp(cmd, "from") == 0) //from
            {
                scanf("%s", tableName);
                scanf("%s", cmd);
                transfer(cmd);
                if (strcmp(cmd, "where") == 0) //where
                {
                    scanf("%s", whereColName);
                    scanf("%s", equal);
                    if (strcmp(equal, "=") == 0) //=
                    {
                        scanf("%s", value);
                        Select(colName, tableName, whereColName, value);
                    }
                    else
                        printf("�����������!\n");
                }
                else
                    printf("�����������!\n");
            }
            else
                printf("�����������!\n");
        }
        else
            printf("�����������,����!\n");
    }
    return 0;
}