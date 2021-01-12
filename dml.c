#include<stdio.h>
#include<string.h>

extern int OpenTable(char *name, PTableMode FieldSet);

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

void split1(char *src,const char *separator,char dest[50][15],int *numstr) {
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
     int i = 0;
     while(pNext != NULL) {
        for(int j = 0;j<strlen(pNext);j++){
            dest[i][j] = pNext[j];
        }
        dest[i][strlen(pNext)]='\0';
          ++count;
         pNext = (char *)strtok(NULL,separator);  //必须使用(char *)进行强制类型转换
         i++;
    }  
    *numstr = count;
} 	

void Insert(char* name,char* value){
    //name字段包含了表名（字段名）...
    //value字段包含了values（字段值）...
    
    int numstr = 0;
    char tname[MAX_SIZE]={0};//用来存放分割之后的表名
    char *tempname[MAX_SIZE]={0};//用来存放中间字段以进行再分配
    char str[100]={0};//用来存放命令字段，以实现再分隔
    char field[50][15]={};//用来存放分割之后的字段名[FIELD_NAME_LENGTH]
    char values[50][15]={};//用来存放分割之后的字段值
    //将name字段的值赋值给str
    for(int i = 0;i<strlen(name);i++){
        str[i] = name[i];
    }
    /* for(int i=strlen(name);i<100;i++){
        str[i] = '\0';
    } */
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
    split1(str,",",field,&numstr);
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
    
    /* for(int i=strlen(name);i<100;i++){
        str[i] = '\0';
    } */
    //printf("%s\n",str);//检查命令是否赋值合格
    //第一轮分割，分割出表values关键词
    split(str,"(",tempname,&numstr);
    for(int i = 0; i<strlen(tempname[1]); i++){
        str[i] = tempname[1][i]; 
    }
    /* for(int i = strlen(tempname[1]);i<100;i++){
        str[i] = '\0';
    } */
    //printf("%s\n",str);//用来检查待实现的命令分割是否合格
    //第二轮分割，分割出纯粹的value值
    split(str,")",tempname,&numstr);
    for(int i = 0; i<strlen(tempname[0]);i++){
        str[i] = tempname[0][i];
    }
    str[strlen(tempname[0])] = '\0';
    //printf("%s\n",str);//用来检查待实现的命令分割是否合格
    //第三轮分割，分割出各个字段的值，保证写入
    split1(str,",",values,&numstr);
    /* for(int i = 0 ;i<numstr;i++){
        printf("%s\n",values[i]);
    } */

    for(int i = 0;i<numstr;i++){
        values[i][strlen(values[i])] = '\40';
        for(int j = strlen(values[i])+1;j<FIELD_NAME_LENGTH;j++){
            values[i][j]='\40';
        }
    }
    //用来检查最终分割字段名是否合格
    /*
    for(int i = 0 ;i<numstr;i++){
        printf("%s\n",values[i]);
        //printf("%p\n",values[i]);//指示指针地址，发现指针地址之差为2，说明存在问题
    }
    */
    
   //获得表结构列表值
    TableMode FieldSet[MAX_SIZE];
    int num = OpenTable(tname, FieldSet);
    if (num == -1)
        printf("No such table!\n");
    else if (num == 0)
        printf("No field in table!\n");
    else
    {
        /*
        printf("FieldName      Type    Size Key  Null\n");
        for (int i = 0; i < num; i++)
            printf("%-15s%-8s%-5d%c    %c\n", FieldSet[i].sFieldName, FieldSet[i].sType,
                   FieldSet[i].iSize, FieldSet[i].bKey, FieldSet[i].bNullFlag);
        */
    }
    //开始写入对应的dat文件
    //创建对应的表，连接得到表名
    strcpy(dat,tname);
    strcat(dat,".dat");
    //printf("%s\n",dat);//检查表名是否连接合格
    dp = fopen(dat, "ab+");
    fwrite("#",sizeof(char), 1, dp);
    for( int i = 0; i<numstr ;i++){
        fwrite(values[i],sizeof(char),FIELD_NAME_LENGTH,dp);
    }
    fclose(dp);
    
    

    //printf("insert into %s %s\n",name,value);
}
void Delete(char* tableName,char* colName,char* value){
    /**
     * 为了实现删除操作
     * 需要做的是读取文件的所有信息
     * 然后通过字符串比对进行处理
     * 然后重新写入处理后的所有数据
     * 这需要创建一个临时文件
     * 读取原文件的一行记录
     * 判定是否需要删除
     * 写入临时文件
     * 全部读取完毕关闭原文件并删除
     * 将临时文件名改为原文件名
    */
    FILE *tp;//用来创建指向的临时文件
    char tname[FIELD_NAME_LENGTH]={0};//用来存放表名
    char tvalue[FIELD_NAME_LENGTH]={0};//用来暂时存放读取的数据
   
    char tflag[2]={0};//用来存放读到的标志位
    for(int i = 0 ;i<strlen(tableName);i++){
        tname[i] = tableName[i];
    }
    strcat(tname,".dat");
    //printf("%s",tname);
    //打开对应的文件
    dp = fopen(tname,"rb+");

    TableMode FieldSet[MAX_SIZE];

    int tnum = OpenTable(tname,FieldSet);

    int flag = 0;
    for(flag = 0;flag < tnum;flag++){
        if(strcmp(colName,FieldSet[flag].sFieldName)==0)//字符串相等
        break;
    }
    
    //读取当前文件中的内容，并且处理文件结果
    while(!feof(dp)){

        tp = fopen("temp.dat","ab+");
        
        fread(tflag,sizeof(char),1,dp);
        //printf("%s",tflag);
        char pvalue[MAX_SIZE][FIELD_NAME_LENGTH]={0};
        for(int i = 0;i<tnum;i++){
            fread(tvalue,sizeof(char),FIELD_NAME_LENGTH,dp);
            printf("%s",tvalue);
            strcpy(pvalue[i],tvalue);
           
        }

        if(strcmp(pvalue[flag],value)!=0){
            fwrite("#",sizeof(char), 1, tp);
            for(int i = 0;i<tnum;i++){
                fwrite(pvalue[i],sizeof(char),FIELD_NAME_LENGTH,tp);
            }
        }
        
        

        fclose(tp);
    }
    fclose(dp);
    remove(tname);
    rename("temp.dat",tname);
    //printf("delete from %s where %s = %s\n",tableName,colName,value);
}
void update(char* tableName,char* newColName,char* newValue,char* colName,char* value){
    /**
     * 为了实现修改操作
     * 需要建立一个临时文件
     * 打开原文件并逐行读取数据
     * 对满足条件的数据进行修改
     * 如果不满足修改条件就直接写入
     * 如果满足修改条件就按照条件修改后写入
     * 写入临时文件
     * 原文件读完之后关闭原文件并删除
     * 将临时文件名称该为原文件名称
    */
    FILE *tp;//用来创建指向的临时文件
    char tname[MAX_SIZE]={0};//用来存放表名
    //复制表名，添加表名后缀
    for(int i = 0 ;i<strlen(tableName);i++){
        tname[i] = tableName[i];
    }
    strcat(tname,".dat");
    //打开文件并准备开始判断和修改操作
    dp = fopen(tname,"rb+");
    while(!feof(dp)){
        tp = fopen("temp.dat","ab+");


        fclose(tp);
    }
    fclose(dp);
    remove(tname);
    rename("temp.dat",tname);
    //printf("update %s = %s from %s where %s = %s\n",tableName,newColName,newValue,colName,value);
}