#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// int execmd(char *cmd, char *result)
// {
//     strcat(cmd, " 2>1.txt"); //���������������1.txt
//     if (system(cmd))
//         return 0; //����0��ʾ����ʧ��
//     FILE *p;
//     if ((p = fopen("1.txt", "r+")) != NULL) //�����ļ�1.txt
//     {
//         fgets(result, 128, p); //�����󴫸�result
//         fclose(p);
//         system("del 1.txt"); //ɾ��1.txt
//         if (strcmp(result, "") == 0)
//             return 1; //����1��ʾ���гɹ�
//         else
//             return 0;
//     }
//     else
//     {
//         fclose(p);
//         return 1; //������1.txt Ĭ��ִ�гɹ�
//     }
// }

void CreateDataBase(char *name)
{
    //printf("create database %s\n", name);

    char ndbf[20]; //�����ݿ��ļ���
    strcpy(ndbf, name);
    strcat(ndbf, ".dbf");

    FILE *p;
    if ((p = fopen(ndbf, "rb")) != NULL) //�����ݿ��Ѵ���
    {
        printf("Database %s already exists!\n", name);
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
    p = NULL;
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

    if (dopens != 0) //�д򿪵����ݿ�
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
        printf("Close database %s successfully!\n", name);
        strcpy(dbf, "");
        fp = NULL;
        dopens = 0;
    }
    else //�ر�ʧ��
    {
        perror("fclose:"); //perror���������һ�������Ĵ�����Ϣ
        printf("Close database %s failed!\n", name);
    }
}

void DropDataBase(char *name)
{
    //printf("drop database %s\n", name);

    char ndbf[20];
    strcpy(ndbf, name);
    strcat(ndbf, ".dbf");

    if (strcmp(ndbf, dbf) == 0) //���ݿ�δ�ر�
    {
        printf("Please close database %s first!\n", name);
        return;
    }

    if (remove(ndbf) == 0) //ɾ���ɹ�
        printf("drop database %s successfully!\n", name);
    else //���ɹ����������
        printf("%s\n", strerror(errno));
}

void RenameDataBase(char *oldName, char *newName)
{
    //printf("rename database %s %s\n", oldName, newName);

    char oldTempdbf[20]; //���ļ���
    strcpy(oldTempdbf, oldName);
    strcat(oldTempdbf, ".dbf");

    char newTempdbf[20]; //���ļ���
    strcpy(newTempdbf, newName);
    strcat(newTempdbf, ".dbf");

    FILE *np = fopen(newTempdbf, "rb");
    if (np != NULL) //����ΪnewTempdbf�����ݿ��Ѵ���
    {
        printf("%s already exists!\n", newTempdbf);
        fclose(np);
        np = NULL;
        return;
    }

    if (strcmp(oldTempdbf, dbf) == 0) //��Ҫ���������ǵ�ǰ���ݿ�
    {
        fclose(fp); //�ر�
        fp = NULL;

        rename(oldTempdbf, newTempdbf);

        fp = fopen(newTempdbf, "rb+"); //��
        strcpy(dbf, newTempdbf);
        printf("Rename database %s %s successfully!\n", oldName, newName);
    }
    else
    {
        FILE *p = fopen(oldTempdbf, "rb");
        if (p == NULL) //û�и����ݿ�
            printf("No such database %s\n", oldName);
        else //�и����ݿ�
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

    char tempdbf[20]; //��ʱdbf�ļ���
    strcpy(tempdbf, name);
    strcat(tempdbf, ".dbf");

    FILE *p;
    if ((p = fopen(tempdbf, "rb")) == NULL) //��ʧ�ܣ�û�и����ݿ�
    {
        printf("No such database!\n");
        return;
    }

    int haveTable = 0;
    while (!feof(p))
    {
        char tempc;
        int i = fread(&tempc, sizeof(char), 1, p);
        if (!i) //��ȡʧ�ܣ�����
        {
            break;
        }
        if (tempc != '~') //�ļ���ʽ�����⣬����
        {
            printf("%s format not correct!\n", tempdbf);
            fclose(p);
            p = NULL;
            return;
        }

        if (!haveTable)
        { //�б������
            haveTable = 1;
            printf("There are tables in database %s:\n", name);
        }

        int num;
        char tname[20];
        fread(tname, sizeof(char), FILE_NAME_LENGTH, p); //��ȡ�����Ϣ
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

    if (dopens == 0) //��ǰδ�����ݿ�
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
        if (tempc != '~') //��ʽ����
        {
            printf("%s format not correct!\n", dbf);
            fseek(fp, 0L, SEEK_SET);
            return -1; //-1��ʾû�ҵ�
        }

        char tempName[20];
        int num = 0;
        fread(tempName, sizeof(char), FILE_NAME_LENGTH, fp); //��ȡ����
        fread(&num, sizeof(int), 1, fp);                     //��ȡ�ֶ���
        fread(FieldSet, sizeof(TableMode), num, fp);         //��ȡ�ṹ
        if (strcmp(tempName, name) == 0)                     //�ҵ��˱�
        {
            topens = 1;
            fseek(fp, 0L, SEEK_SET);
            return num; //�����ֶθ���
        }
    }
    fseek(fp, 0L, SEEK_SET);
    return -1;
}

void RenameTable(char *oldName, char *newName)
{
    //printf("rename table %s %s\n", oldName, newName);

    if (dopens == 0) //û�д򿪵����ݿ�
    {
        printf("Please open database first!\n");
        return;
    }

    TableMode temp[MAX_SIZE];
    if (OpenTable(newName, temp) != -1) //�ж��±��Ƿ��Ѵ���
    {
        printf("Table %s already exists!\n", newName);
        return;
    }

    while (!feof(fp))
    {
        char tempc;
        int i = fread(&tempc, sizeof(char), 1, fp);
        if (!i) //��ȡʧ�ܣ�����ѭ��
        {
            break;
        }

        if (tempc != '~') //�ļ���ʽ�����⣬����
        {
            printf("%s format not correct!\n", dbf);
            fseek(fp, 0L, SEEK_SET);
            return;
        }

        int num;
        char tname[20];
        fread(tname, sizeof(char), FILE_NAME_LENGTH, fp); //��ȡ�����Ϣ
        if (strcmp(oldName, tname) == 0)                  //�ҵ��˸ñ�͸���
        {
            fseek(fp, long((-1) * sizeof(char) * FILE_NAME_LENGTH), SEEK_CUR);
            fwrite(newName, sizeof(char), FILE_NAME_LENGTH, fp);
            printf("rename table %s %s successfully!\n", oldName, newName);
            fseek(fp, 0L, SEEK_SET);
            return;
        }
        fread(&num, sizeof(int), 1, fp);
        fseek(fp, long(sizeof(TableMode) * num), SEEK_CUR);
    }
    printf("No such table in database %s\n", dbf);
    fseek(fp, 0L, SEEK_SET);
}

void RenameField(char *oldName, char *newName, char *tableName)
{
    printf("rename field %s %s in %s\n", oldName, newName, tableName);
}

void CreateTable(char *name)
{
    //printf("create table %s\n", name);
    //fp = fopen("data.dbf", "ab+");

    if (fp == NULL || dopens == 0) //��ǰδ�����ݿ�
    {
        printf("No database is open!\nPlease open database first!\n");
        return;
    }

    TableMode tempTable[MAX_SIZE];
    if (OpenTable(name, tempTable) == -1) //-1��ʾû�иñ����Խ�
        topens = 0;
    else //���Ѵ��ڣ����ܽ�
    {
        printf("Table %s already exist!\n", name);
        return;
    }

    fseek(fp, 0L, SEEK_END); //�ƶ�ָ�뵽�ļ�β
    char temp[10];
    scanf("%s", temp);
    if (strcmp(temp, "(") == 0) //(
    {
        TableMode FieldSet[MAX_SIZE];
        int num = 0;
        while (1)
        {
            scanf("%15s%8s%d", FieldSet[num].sFieldName, FieldSet[num].sType, &(FieldSet[num].iSize)); //��ȡ�ֶ������ֶ����ͣ��ֶ��ֳ�
            if (strcmp(FieldSet[num].sType, "char") == 0)                                              //�ֶ�������char
                ;
            else if (strcmp(FieldSet[num].sType, "int") == 0 || strcmp(FieldSet[num].sType, "float") == 0) //�ֶ�������int����float
            {
                if (FieldSet[num].iSize != 1)
                    FieldSet[num].iSize = 1; //�ֶγ��Ȳ�Ϊ1
            }
            else //�ֶ����Ͳ�֧��
            {
                printf("Fieldtype must in (\"int\",\"char\",\"float\")!\n");
                printf("�����������!\n");
                fflush(stdin);
                return;
            }

            char tempKey[10];
            scanf("%s", tempKey); //��ȡ�ֶ��Ƿ�Ϊ����
            FieldSet[num].bKey = tempKey[0];
            if (FieldSet[num].bKey != 'n' && FieldSet[num].bKey != 'y') //����bKey���Ϸ�
            {
                printf("�����������!\n");
                fflush(stdin);
                return;
            }
            //printf("%s%s%d%c", FieldSet[num].sFieldName, FieldSet[num].sType, FieldSet[num].iSize, FieldSet[num].bKey);

            char tempNull[10];
            scanf("%s", tempNull); //��ȡ�ֶ��Ƿ�Ϊ��
            FieldSet[num].bNullFlag = tempNull[0];
            if (FieldSet[num].bNullFlag != 'n' && FieldSet[num].bNullFlag != 'y') //����bNullFlag���Ϸ�
            {
                printf("�����������!\n");
                fflush(stdin);
                return;
            }

            FieldSet[num].bValidFlag = 'y'; //Ĭ���ֶ���Ч
            num++;
            if (tempNull[1] == ',') //����ж���˵��������һ���ֶΣ���������ѭ��
                continue;
            else
                break;
        }
        scanf("%s", temp);
        if (strcmp(temp, ")") == 0) //) ��ʽ��ȷ���������Ϣ
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
            printf("�����������!\n");
            fflush(stdin);
        }
    }
    else
    {
        printf("�����������!\n");
        fflush(stdin);
    }
}

void DropTable(char *name)
{
    //printf("drop table %s\n", name);

    if (fp == NULL || dopens == 0) //��ǰδ�����ݿ�
    {
        printf("No database is open!\nPlease open database first!\n");
        return;
    }

    fseek(fp, 0L, SEEK_SET);
    int find = 0; //0��ʾδ�ҵ���1��ʾ�ҵ�
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
        if (tempc != '~') //��ʽ����
        {
            printf("%s format not correct or %s is NULL!\n", dbf, dbf);
            fseek(fp, 0L, SEEK_SET);
            return;
        }

        fread(tempName, sizeof(char), FILE_NAME_LENGTH, fp); //��ȡ����
        fread(&num, sizeof(int), 1, fp);                     //��ȡ�ֶ���
        fread(FieldSet, sizeof(TableMode), num, fp);         //��ȡ�ṹ
        if (strcmp(tempName, name) == 0)                     //�ҵ��˱�
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
                fread(&tempc, sizeof(char), 1, fp); //��ȡ~
                if (tempc != '~')
                    break;
                fread(tempName, sizeof(char), FILE_NAME_LENGTH, fp); //��ȡ����
                fread(&temp, sizeof(int), 1, fp);                    //��ȡ�ֶ���
                fread(tempField, sizeof(TableMode), temp, fp);       //��ȡ�ṹ

                if (strcmp(tempName, name) == 0) //����Ǳ�ɾ����������
                    continue;
                else
                {
                    fwrite(&tempc, sizeof(char), 1, np);                  //д��~
                    fwrite(tempName, sizeof(char), FILE_NAME_LENGTH, np); //д�����
                    fwrite(&temp, sizeof(int), 1, np);                    //д���ֶ���
                    fwrite(tempField, sizeof(TableMode), temp, np);       //д��ṹ
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
            if (remove(tName) == 0) //ɾ���ɹ�
                ;
            else //���ɹ����������
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
    topens = 0;

    if (num == -1) //-1��ʾû�ҵ���
        printf("No such table!\n");
    else if (num == 0) //0��ʾû���ֶ�
        printf("No field in table!\n");
    else
    {
        printf("FieldName      Type    Size Key  Null Valid\n"); //�����Ϣ
        for (int i = 0; i < num; i++)
            printf("%-15s%-8s%-5d%c    %c    %c\n", FieldSet[i].sFieldName, FieldSet[i].sType,
                   FieldSet[i].iSize, FieldSet[i].bKey, FieldSet[i].bNullFlag, FieldSet[i].bValidFlag);
    }
}
