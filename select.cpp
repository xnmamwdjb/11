#include<stdio.h>
#include<string.h>

void Select(char* colName, char* tableName, char* whereColName, char* value)
{
  int num= 0;
  char *tname[MAX_SIZE]={0};
  char *buffer[MAX_SIZE][FIELD_NAME_LENGTH];
  char *temp,*temp1[MAX_SIZE]={0};
  int numstr,i=0,j,temp2,temp3,count,n;
  split(tableName,",",tname,&num);//分离表名
  /*if (fp == NULL || dopens == 0) //当前未打开数据库
  {
      printf("No database is open!\n");
      return;
  }*/
  switch(num)
  {
    case '1':
      strcpy(dat,tname[0]);
      strcat(dat,".dat");
      TableMode tempTable[MAX_SIZE];
      numstr=OpenTable(tname[0], tempTable);//获取表的字段数量
      if (numstr== -1)
      {
        printf("no table names %s",tname[0]);
        return;
      }
      dp=fopen(dat,"rb");
      while (!feof(fp)){
        fread(temp,sizeof(char),numstr,dp);//一次读取一行数据
        split(temp," ",temp1,&n);
        for(j=0;j<numstr;j++){
          buffer[i][j]=temp1[j];//将读取的数据存入
          if(buffer[0][j]==whereColName){
            temp2=j;//找到条件列的下标
          }
          if(buffer[0][j]==colName){
            temp3=j;//目标列的下标
          }
        }
        i++;
        count++;
      }
      fseek(dp, 0L, SEEK_SET);
      for(i=0;i<count;i++){
        if(buffer[i][temp2]==value){
          printf("%s\n",colName);
          printf("%s\n",buffer[i][temp3]);//目标值
          break;
        }
      }
      fclose(dp);
        /*for(i=0;i<numstr;i++){
          for(j=0;j<)
        }*/
  }
}
