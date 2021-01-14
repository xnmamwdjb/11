#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void CreateDataBase(char *name)
{
    //printf("create database %s\n", name);

    char ndbf[20]; //新数据库文件名
    strcpy(ndbf, name);
    strcat(ndbf, ".dbf");

    FILE *p;
    if ((p = fopen(ndbf, "rb")) != NULL) //该数据库已存在
    {
        printf("Database %s already exists!\n", name);
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
    p = NULL;
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

    if (dopens != 0) //有打开的数据库
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
        printf("Close database %s successfully!\n", name);
        strcpy(dbf, "");
        fp = NULL;
        dopens = 0;
    }
    else //关闭失败
    {
        perror("fclose:"); //perror用于输出上一个函数的错误信息
        printf("Close database %s failed!\n", name);
    }
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

    if (remove(ndbf) == 0) //删除成功
        printf("drop database %s successfully!\n", name);
    else //不成功，输出错误
        printf("%s\n", strerror(errno));
}

void RenameDataBase(char *oldName, char *newName)
{
    //printf("rename database %s %s\n", oldName, newName);

    char oldTempdbf[20]; //旧文件名
    strcpy(oldTempdbf, oldName);
    strcat(oldTempdbf, ".dbf");

    char newTempdbf[20]; //新文件名
    strcpy(newTempdbf, newName);
    strcat(newTempdbf, ".dbf");

    FILE *np = fopen(newTempdbf, "rb");
    if (np != NULL) //名字为newTempdbf的数据库已存在
    {
        printf("%s already exists!\n", newTempdbf);
        fclose(np);
        np = NULL;
        return;
    }

    if (strcmp(oldTempdbf, dbf) == 0) //需要重命名的是当前数据库
    {
        fclose(fp); //关闭
        fp = NULL;

        rename(oldTempdbf, newTempdbf);

        fp = fopen(newTempdbf, "rb+"); //打开
        strcpy(dbf, newTempdbf);
        printf("Rename database %s %s successfully!\n", oldName, newName);
    }
    else
    {
        FILE *p = fopen(oldTempdbf, "rb");
        if (p == NULL) //没有该数据库
            printf("No such database %s\n", oldName);
        else //有该数据库
        {
            fclose(p);
            p = NULL;
            rename(oldTempdbf, newTempdbf);
            printf("rename database %s %s successfully!\n", oldName, newName);
        }
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
    {
        printf("No such database!\n");
        return;
    }

    int haveTable = 0;
    while (!feof(p))
    {
        char tempc;
        int i = fread(&tempc, sizeof(char), 1, p);
        if (!i) //读取失败，返回
        {
            break;
        }
        if (tempc != '~') //文件格式有问题，返回
        {
            printf("%s format not correct!\n", tempdbf);
            fclose(p);
            p = NULL;
            return;
        }

        if (!haveTable)
        { //有表则输出
            haveTable = 1;
            printf("There are tables in database %s:\n", name);
        }

        int num;
        char tname[20];
        fread(tname, sizeof(char), FILE_NAME_LENGTH, p); //读取表格信息
        fread(&num, sizeof(int), 1, p);
        fseek(p, long(sizeof(TableMode) * num), SEEK_CUR);
        printf("%s\n", tname);
    }
    if (haveTable == 0)
        printf("No table in database %s!\n", name);
    fclose(p);
    p = NULL;
}

int OpenTable(char *name, PTableMode FieldSet)
{
    //printf("open table %s\n", name);
    //fp = fopen("data.dbf", "rb+");

    if (dopens == 0) //当前未打开数据库
    {
        printf("No database is open!\nPlease open database first!\n");
        return -1;
    }

    fseek(fp, 0L, SEEK_SET);

    while (!feof(fp))
    {
        char tempc;
        int i = fread(&tempc, sizeof(char), 1, fp);
        if (!i)
        {
            break;
        }
        if (tempc != '~') //格式不对
        {
            printf("%s format not correct!\n", dbf);
            fseek(fp, 0L, SEEK_SET);
            return -1; //-1表示没找到
        }

        char tempName[20];
        int num = 0;
        fread(tempName, sizeof(char), FILE_NAME_LENGTH, fp); //读取表名
        fread(&num, sizeof(int), 1, fp);                     //读取字段数
        if (strcmp(tempName, name) == 0)                     //找到了表
        {
            topens = 1;
            fread(FieldSet, sizeof(TableMode), num, fp); //读取结构
            fseek(fp, 0L, SEEK_SET);
            return num; //返回字段个数
        }
        else
            fseek(fp, sizeof(TableMode) * num, SEEK_CUR); //不是需要的表就跳过
    }
    fseek(fp, 0L, SEEK_SET);
    return -1;
}

void RenameTable(char *oldName, char *newName)
{
    //printf("rename table %s %s\n", oldName, newName);

    if (dopens == 0) //没有打开的数据库
    {
        printf("Please open database first!\n");
        return;
    }

    TableMode temp[MAX_SIZE];
    if (OpenTable(newName, temp) != -1) //判断新表是否已存在
    {
        printf("Table %s already exists!\n", newName);
        return;
    }
    topens = 0;

    while (!feof(fp))
    {
        char tempc;
        int i = fread(&tempc, sizeof(char), 1, fp);
        if (!i) //读取失败，跳出循环
        {
            break;
        }

        if (tempc != '~') //文件格式有问题，返回
        {
            printf("%s format not correct!\n", dbf);
            fseek(fp, 0L, SEEK_SET);
            return;
        }

        int num;
        char tname[20];
        fread(tname, sizeof(char), FILE_NAME_LENGTH, fp); //读取表格信息
        if (strcmp(oldName, tname) == 0)                  //找到了该表就改名
        {
            fseek(fp, long((-1) * sizeof(char) * FILE_NAME_LENGTH), SEEK_CUR);
            fwrite(newName, sizeof(char), FILE_NAME_LENGTH, fp);
            printf("rename table %s %s successfully!\n", oldName, newName);

            char olddat[20],newdat[20];
            strcpy(olddat,oldName);
            strcat(olddat,".dat");
            strcpy(newdat,newName);
            strcat(newdat,".dat");
            rename(olddat,newdat);
            
            fseek(fp, 0L, SEEK_SET);
            return;
        }
        fread(&num, sizeof(int), 1, fp);
        fseek(fp, long(sizeof(TableMode) * num), SEEK_CUR);
    }
    printf("No such table in database %s\n", dbf); //循环结束，说明没找到表
    fseek(fp, 0L, SEEK_SET);
}

void RenameField(char *oldName, char *newName, char *tableName)
{
    //printf("rename field %s %s in %s\n", oldName, newName, tableName);

    TableMode FieldSet[MAX_SIZE];
    int num = OpenTable(tableName, FieldSet);
    topens = 0;
    if (num == -1) //没有该表
    {
        printf("No such table!\n");
        return;
    }

    TableMode tempField;
    int position = -1;
    int i = 0;
    for (i = 0; i < num; i++) //寻找字段新旧名字是否存在
    {
        if (strcmp(FieldSet[i].sFieldName, oldName) == 0) //找到了则将新的名字赋给tempField
        {
            position = i;
            strcpy(FieldSet[i].sFieldName, newName);
        }
        else if (strcmp(FieldSet[i].sFieldName, newName) == 0)
            break;
    }
    if (i != num) //执行了break，说明新字段名在fieldset中已存在
        printf("Field %s already exists!\n", newName);
    else
    {
        if (position == -1) //position没变，说明没找到旧字段
            printf("No such Field in %s!\n", tableName);
        else
        {
            while (!feof(fp))
            {
                char tempc;
                int i = fread(&tempc, sizeof(char), 1, fp);
                if (!i) //读取失败，跳出循环
                {
                    break;
                }

                if (tempc != '~') //文件格式有问题，返回
                {
                    printf("%s format not correct!\n", dbf);
                    fseek(fp, 0L, SEEK_SET);
                    return;
                }

                int num;
                char tname[20];
                fread(tname, sizeof(char), FILE_NAME_LENGTH, fp); //读取表格信息
                fread(&num, sizeof(int), 1, fp);
                if (strcmp(tableName, tname) == 0) //找到了该表,指向该表tablemode的第一位
                {
                    fseek(fp, long(sizeof(TableMode) * position), SEEK_CUR); //跳过position个字段结构
                    fwrite(&FieldSet[position], sizeof(TableMode), 1, fp);
                    printf("rename field %s %s in %s successfully!\n", oldName, newName, tableName);
                    fseek(fp, 0L, SEEK_SET);
                    return;
                }
                fseek(fp, long(sizeof(TableMode) * num), SEEK_CUR);
            }
        }
    }
}

void CreateTable(char *name)
{
    //printf("create table %s\n", name);
    //fp = fopen("data.dbf", "ab+");

    if (dopens == 0) //当前未打开数据库
    {
        printf("No database is open!\nPlease open database first!\n");
        return;
    }

    TableMode tempTable[MAX_SIZE];
    if (OpenTable(name, tempTable) == -1) //-1表示没有该表，可以建
        topens = 0;
    else //表已存在，不能建
    {
        printf("Table %s already exist!\n", name);
        return;
    }

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

            for (int i = 0; i < num; i++) //判断字段是否已存在
            {
                if (strcmp(FieldSet[i].sFieldName, FieldSet[num].sFieldName) == 0)
                {
                    printf("Field %s already exists!\n", FieldSet[num].sFieldName);
                    fflush(stdin);
                    return;
                }
            }

            if (strcmp(FieldSet[num].sType, "char") == 0) //字段类型是char
                ;
            else if (strcmp(FieldSet[num].sType, "int") == 0 || strcmp(FieldSet[num].sType, "double") == 0) //字段类型是int或者double
            {
                if (FieldSet[num].iSize != 1)
                    FieldSet[num].iSize = 1; //字段长度不为1
            }
            else //字段类型不支持
            {
                printf("Fieldtype must in (\"int\",\"char\",\"double\")!\n");
                printf("命令语句有误!\n");
                fflush(stdin);
                return;
            }

            char tempKey[10];
            scanf("%s", tempKey); //读取字段是否为主键
            FieldSet[num].bKey = tempKey[0];
            if (FieldSet[num].bKey != 'n' && FieldSet[num].bKey != 'y') //输入bKey不合法
            {
                printf("命令语句有误!\n");
                fflush(stdin);
                return;
            }
            //printf("%s%s%d%c", FieldSet[num].sFieldName, FieldSet[num].sType, FieldSet[num].iSize, FieldSet[num].bKey);

            char tempNull[10];
            scanf("%s", tempNull); //读取字段是否为空
            FieldSet[num].bNullFlag = tempNull[0];
            if (FieldSet[num].bNullFlag != 'n' && FieldSet[num].bNullFlag != 'y') //输入bNullFlag不合法
            {
                printf("命令语句有误!\n");
                fflush(stdin);
                return;
            }

            FieldSet[num].bValidFlag = 'y'; //默认字段有效
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
            fseek(fp, 0L, SEEK_SET);
            printf("create table %s successfully!\n", name);
        }
        else
        {
            printf("命令语句有误!\n");
            fflush(stdin);
        }
    }
    else
    {
        printf("命令语句有误!\n");
        fflush(stdin);
    }
}

void DropTable(char *name)
{
    //printf("drop table %s\n", name);

    if (dopens == 0) //当前未打开数据库
    {
        printf("No database is open!\nPlease open database first!\n");
        return;
    }

    fseek(fp, 0L, SEEK_SET);
    int find = 0; //0表示未找到，1表示找到
    while (!feof(fp))
    {
        char tempc;
        char tempName[20];
        int num;
        TableMode FieldSet[MAX_SIZE];
        int i = fread(&tempc, sizeof(char), 1, fp);
        if (!i)
        {
            break;
        }
        if (tempc != '~') //格式不对
        {
            printf("%s format not correct or %s is NULL!\n", dbf, dbf);
            fseek(fp, 0L, SEEK_SET);
            return;
        }

        fread(tempName, sizeof(char), FILE_NAME_LENGTH, fp); //读取表名
        fread(&num, sizeof(int), 1, fp);                     //读取字段数
        fread(FieldSet, sizeof(TableMode), num, fp);         //读取结构
        if (strcmp(tempName, name) == 0)                     //找到了表
        {
            find = 1;
            fseek(fp, 0L, SEEK_SET);
            break;
        }
    }

    if (find == 0)
        printf("No such table in database %s\n", dbf);
    else
    {
        FILE *np;
        if ((np = fopen("temp.dbf", "ab+")) == NULL)
        {
            printf("Open or create file error!\n");
        }

        else
        {
            while (!feof(fp))
            {
                char tempc, tempName[20];
                int temp;
                TableMode tempField[MAX_SIZE];
                fread(&tempc, sizeof(char), 1, fp); //读取~
                if (tempc != '~')
                    break;
                fread(tempName, sizeof(char), FILE_NAME_LENGTH, fp); //读取表名
                fread(&temp, sizeof(int), 1, fp);                    //读取字段数
                fread(tempField, sizeof(TableMode), temp, fp);       //读取结构

                if (strcmp(tempName, name) == 0) //如果是被删除表则跳过
                    continue;
                else
                {
                    fwrite(&tempc, sizeof(char), 1, np);                  //写入~
                    fwrite(tempName, sizeof(char), FILE_NAME_LENGTH, np); //写入表名
                    fwrite(&temp, sizeof(int), 1, np);                    //写入字段数
                    fwrite(tempField, sizeof(TableMode), temp, np);       //写入结构
                }
            }

            fclose(fp);
            fp = NULL;
            fclose(np);
            np = NULL;
            remove(dbf);
            rename("temp.dbf", dbf);

            char tName[20];
            strcpy(tName, name);
            strcat(tName, ".dat");
            if (remove(tName) == 0) //删除成功
                ;
            else //不成功，输出错误
                printf("%s\n", strerror(errno));

            if ((fp = fopen(dbf, "rb+")) == NULL)
            {
                printf("Open file error!\n");
                dopens = 0;
                strcpy(dbf, "");
            }
            else
                printf("Drop table %s successfully!\n", name);
        }
    }
    fseek(fp, 0L, SEEK_SET);
}

void ViewTable(char *name)
{
    //printf("view table %s\n", name);
    TableMode FieldSet[MAX_SIZE];
    int num = OpenTable(name, FieldSet);
    //topens = 0;

    if (num == -1) //-1表示没找到表
        printf("No such table!\n");
    else if (num == 0) //0表示没有字段
        printf("No field in table!\n");
    else
    {
        printf("FieldName      Type    Size Key  Null Valid\n"); //输出信息
        for (int i = 0; i < num; i++)
            printf("%-15s%-8s%-5d%c    %c    %c\n", FieldSet[i].sFieldName, FieldSet[i].sType,
                   FieldSet[i].iSize, FieldSet[i].bKey, FieldSet[i].bNullFlag, FieldSet[i].bValidFlag);
    }
}
