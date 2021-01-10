#include <stdio.h>
#include <string.h>

FILE *fp;
char dbf[20];

#define MAX_SIZE 50          //表中所含字段最大长度
#define FILE_NAME_LENGTH 15  //文件名最大长度
#define FIELD_NAME_LENGTH 15 //字段名最大长度

typedef struct
{
    char sFieldName[FIELD_NAME_LENGTH]; //字段名
    char sType[8]; //字段类型
    int iSize; //字段字长
    char bKey; //字段是否是KEY键
    char bNullFlag; //字段是否允许为空
    char bValidFlag; //字段是否有效
} TableMode, *PTableMode; //字段结构

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
    printf("请按以下规则输入命令语句(命令不分大小写)\n");
    printf("新建数据库: create database 数据库名\n");
    printf("删除数据库: drop database 数据库名\n");
    printf("打开数据库: open database 数据库名\n");
    printf("关闭数据库: close database 数据库名\n");
    printf("添加新表  : create table  表名\n");
    printf("            (\n");
    printf("            字段名 数据类型 字长 是否为KEY键（y/n） 是否可空（y/n）,\n");
    printf("            字段名 数据类型 字长 是否为KEY键（y/n） 是否可空（y/n）,\n");
    printf("            …          …\n");
    printf("            )\n");
    printf("查看表结构: open table 表名\n");
    printf("删除旧表  : drop table 表名\n");
    printf("查询      : select 列名(all表示所有列) from 表名 where 列名 = 值(值可为all,表全部范围)\n");
    printf("插入      : insert into 表名(field1,field2,…) values(value1,value2,…)\n");
    printf("修改      : update 列名 = 新值 from 表名 where 列名 = 值(值可为all,表全部范围)\n");
    printf("删除      : delete from 表名 where 列名 = 值(值可为all,表全部范围)\n");
    printf("退出程序  : exit\n");
    char cmd[10];
    while (~scanf("%s", cmd))
    {
        transfer(cmd);
        if (strcmp(cmd, "exit") == 0) //exit
            break;
        if (strcmp(cmd, "create") == 0) //create
        {
            //create database 数据库名
            //create table 表名
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
                printf("命令语句有误!\n");
        }
        else if (strcmp(cmd, "drop") == 0) //drop
        {
            //drop database 数据库名
            //drop table 表名
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
                printf("命令语句有误!\n");
        }
        else if (strcmp(cmd, "open") == 0) //open
        {
            //open database 数据库名
            //open table 表名
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
                printf("命令语句有误!\n");
        }
        else if (strcmp(cmd, "close") == 0) //close
        {
            //close 数据库名
            char name[20];
            scanf("%s", cmd);
            transfer(cmd);
            if (strcmp(cmd, "database") == 0) //database
            {
                scanf("%s", name);
                CloseDataBase(name);
            }
            else
                printf("命令语句有误!\n");
        }
        else if (strcmp(cmd, "insert") == 0) //insert
        {
            //insert into 表名(field1,field2,…) values(value1,value2,…)"
            char value[100], name[100];//name是表名加上相应的字段名称，需要二次处理保留其中的值，values也需要二次处理
            scanf("%s", cmd);
            transfer(cmd);
            if (strcmp(cmd, "into") == 0) //into
            {
                scanf("%s%s", name, value);
                Insert(name, value);
            }
            else
                printf("命令语句有误!\n");
        }
        else if (strcmp(cmd, "delete") == 0) //delete
        {
            //delete from 表名 where 列名 = 值(值可为all, 表全部范围)
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
                        printf("命令语句有误!\n");
                }
                else
                    printf("命令语句有误!\n");
            }
            else
                printf("命令语句有误!\n");
        }
        else if (strcmp(cmd, "update")==0) //update
        {
            //update 列名 = 新值 from 表名 where 列名 = 值(值可为all, 表全部范围)
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
                            printf("命令语句有误1!\n");
                    }
                    else
                        printf("命令语句有误2!\n");
                }
                else
                    printf("命令语句有误3!\n");
            }
            else
                printf("命令语句有误4!\n");
        }
        else if (strcmp(cmd, "select")==0) //select
        {
            //select 列名(all表示所有列)  from 表名 where 列名 = 值(值可为all,表全部范围)"
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
                        printf("命令语句有误!\n");
                }
                else
                    printf("命令语句有误!\n");
            }
            else
                printf("命令语句有误!\n");
        }
        else
            printf("输入命令错误,请检查!\n");
    }
    return 0;
}