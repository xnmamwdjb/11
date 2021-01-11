#include<stdio.h>
#include<string.h>
#include"ddl.h"


//分割字符串函数，用来实现对指定字符的分割
void split(char *src,const char *separator,char **dest,int *numstr) {
	/*
		src 源字符串的首地址(buf的地址) 
		separator 指定的分割字符
		dest 接收子字符串的数组
		numstr 分割后子字符串的个数
	*/
     char *pNext;
     int count = 0;
     if (src == NULL || strlen(src) == 0) //如果传入的地址为空或长度为0，直接终止 
        return;
     if (separator == NULL || strlen(separator) == 0) //如未指定分割的字符串，直接终止 
        return;
     pNext = (char *)strtok(src,separator); //必须使用(char *)进行强制类型转换(虽然不写有的编译器中不会出现指针错误)
     while(pNext != NULL) {
          *dest++ = pNext;
          ++count;
         pNext = (char *)strtok(NULL,separator);  //必须使用(char *)进行强制类型转换
    }  
    *numstr = count;
} 	

void Insert(char* name,char* value){
    //name字段包含了表名（字段名）...
    //value字段包含了values（字段值）...
    int numstr = 0;
    char tname[MAX_SIZE]={0};//用来存放分割之后的表名
    char *tempname[FIELD_NAME_LENGTH]={0};//用来存放中间字段以进行再分配
    char str[(MAX_SIZE+1)*FIELD_NAME_LENGTH+2]={0};//用来存放命令字段，以实现再分隔
    char *field[FIELD_NAME_LENGTH]={0};//用来存放分割之后的字段名
    char *values[FIELD_NAME_LENGTH]={0};//用来存放分割之后的字段值
    //将name字段的值赋值给str
    for(int i = 0;i<strlen(name);i++){
        str[i] = name[i];
    }
    //printf("%s\n",str);//检查命令是否赋值合格
    //第一轮分割，分割出表名
    split(str,"(",tempname,&numstr);
    for(int i = 0; i<strlen(tempname[0]);i++){
        tname[i] = tempname[0][i]; 
    }
    for(int i = 0; i<strlen(tempname[1]); i++){
        str[i] = tempname[1][i]; 
    }
    str[strlen(tempname[1])] = '\0';
    //printf("%s\n",str);//用来检查待实现的命令分割是否合格
    //第二轮分割，分割出纯粹的field值
    split(str,")",tempname,&numstr);
    for(int i = 0; i<strlen(tempname[0]);i++){
        str[i] = tempname[0][i];
    }
    str[strlen(tempname[0])] = '\0';
    //printf("%s\n",str);//用来检查待实现的命令分割是否合格
    //第三轮分割，分割出各个字段的值，保证写入
    split(str,",",field,&numstr);
    //用来检查最终分割字段名是否合格
    /*
    for(int i = 0 ;i<numstr;i++){
        printf("%s\n",field[i]);
    }
    */
    //将values字段的值赋值给str
    for(int i = 0;i<strlen(value);i++){
        str[i] = value[i];
    }
    //printf("%s\n",str);//检查命令是否赋值合格
    //第一轮分割，分割出表values关键词
    split(str,"(",tempname,&numstr);
    for(int i = 0; i<strlen(tempname[1]); i++){
        str[i] = tempname[1][i]; 
    }
    str[strlen(tempname[1])] = '\0';
    //printf("%s\n",str);//用来检查待实现的命令分割是否合格
    //第二轮分割，分割出纯粹的value值
    split(str,")",tempname,&numstr);
    for(int i = 0; i<strlen(tempname[0]);i++){
        str[i] = tempname[0][i];
    }
    str[strlen(tempname[0])] = '\0';
    //printf("%s\n",str);//用来检查待实现的命令分割是否合格
    //第三轮分割，分割出各个字段的值，保证写入
    split(str,",",values,&numstr);
    //用来检查最终分割字段名是否合格
    /*
    for(int i = 0 ;i<numstr;i++){
        printf("%s\n",values[i]);
    }
    */
    //开始写入对应的dat文件
    //创建对应的表，连接得到表名
    strcpy(dat,tname);
    strcat(dat,".dat");
    //printf("%s\n",dat);//检查表名是否连接合格
    dp = fopen(dat, "ab+");
    fwrite("#",sizeof(char), 1, dp);
    for( int i = 0; i<numstr ;i++){
        fwrite(values[i],strlen(values[i])+1,1,dp);
    }
    fclose(dp);
    


    //printf("insert into %s %s\n",name,value);
}
void Delete(char* tableName,char* colName,char* value){
    printf("delete from %s where %s = %s\n",tableName,colName,value);
}
void update(char* tableName,char* newColName,char* newValue,char* colName,char* value){
    printf("update %s = %s from %s where %s = %s\n",tableName,newColName,newValue,colName,value);
}