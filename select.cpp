#include<stdio.h>
#include<string.h>

void Select(char* colName, char* tableName, char* whereColName, char* value)
{
  int num= 0,tempi;
  char *tname[MAX_SIZE]={0};
  char *buffer[MAX_SIZE][FIELD_NAME_LENGTH];
  char temp,*temp1[MAX_SIZE]={0};
  int numstr,i=0,j,temp2,temp3,temp4,count,n;
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
      printf("%s",dat);
      strcat(dat,".dat");
      TableMode tempTable[MAX_SIZE];
      numstr=OpenTable(tname[0], tempTable);//获取表的字段数量
      if (numstr== -1)
      {
        printf("no table names %s",tname[0]);
        return;
      }
  
      dp=fopen(dat,"rb");
      tempi=fread(&temp,1,1,dp);
      printf("%c\n",temp);
      if (!tempi||temp!='#'){
        printf("no date in table names %s",*tname);
        return;
      }
      while(!feof(dp)){
        for(j=0;j<numstr;j++){
          fread(temp1,tempTable[j].iSize,1,dp);
          if(tempTable[j].sFieldName==whereColName){

          }
          //rintf("%s\n",*tempc);
          buffer[i][j]=*temp1;
        }
        i++;
        count++;//记录记录数量
        tempi=fread(&temp,1,1,dp);
        if(temp!='#'||!tempi){
          break;
        }
      }
      for(i=0;i<numstr;i++){
        if(tempTable[i].sFieldName==colName){
          temp2=i;
        }
        if(tempTable[i].sFieldName==whereColName){
          temp3=i;
        }
      }
      for(j=0;j<count;j++){
        if(buffer[j][temp3]==value){
          temp4=j;
          break;
        }
      }
      printf("%s\n",colName);
      printf("%s\n",*buffer[temp4][temp2]);
      fclose(dp);
}
