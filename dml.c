#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

extern int OpenTable(char *name, PTableMode FieldSet);

/**分割字符串函数，用来实现对指定字符的分割,指针类型*/
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

/**分割字符串函数，数组类型*/
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

/**插入函数，实现对数据按照表结构的插入功能*/
void Insert(char* name,char* value){
    //name字段包含了表名（字段名）...
    //value字段包含了values（字段值）...
    /*第一步，检查是否打开数据库和表，如果未打开，则报错*/
    if(dopens == 0){
        printf("数据库未打开，无法执行插入操作!\n");
    }
    else if(topens == 0){
        printf("表未打开，无法执行插入操作！\n");
    }
    else//检测有效，可以开始获取指令并录入信息
    {
    /*第二步，获得命令字段并分割命令字段*/
    int numstr = 0;//用来得到命令字段分割之后的总个数
    char tname[MAX_SIZE]={0};//用来存放分割之后的表名
    char *tempname[MAX_SIZE]={0};//用来存放中间字段以进行再分配
    char str[100]={0};//用来存放命令字段，以实现再分隔
    char field[50][15]={};//用来存放分割之后的字段名[FIELD_NAME_LENGTH]
    char values[50][15]={};//用来存放分割之后的字段值
    //将name字段的值赋值给str
    strcpy(str,name);
    //第一轮分割，分割出表名
    split(str,"(",tempname,&numstr);
    for(int i = 0; i<strlen(tempname[0]);i++){
        tname[i] = tempname[0][i]; 
    }
    strcpy(str,tempname[1]);
    //第二轮分割，分割出纯粹的field值
    split(str,")",tempname,&numstr);
    strcpy(str,tempname[0]);
    //第三轮分割，分割出各个字段的值，保证写入
    split1(str,",",field,&numstr);
    //将values字段的值赋值给str
    strcpy(str,value);
    //第一轮分割，分割出表values关键词
    split(str,"(",tempname,&numstr);
    strcpy(str,tempname[1]);
    //第二轮分割，分割出纯粹的value值
    split(str,")",tempname,&numstr);
    strcpy(str,tempname[0]);
    //第三轮分割，分割出各个字段的值，保证写入
    split1(str,",",values,&numstr);
   /*获得表结构列表值*/
    TableMode FieldSet[MAX_SIZE];
    int num = OpenTable(tname, FieldSet);//局部变量，用来得到表的字段个数值
    if (num == -1){
        printf("No such table!\n");
        return;
    }
    else if (num == 0)
        printf("No field in table!\n");
    else
    {
        /*处理刚才的命令值，使的命令按照表的顺序进行预处理*/
        char fieldname[MAX_SIZE][FIELD_NAME_LENGTH]={0};//用来存放表名中读取得到的字段做临时匹配，原因在于命令写入的时候字段顺序可能是随机的
        char forinsertvalue[MAX_SIZE][FIELD_NAME_LENGTH]={0};//用来按照顺序存放待插入的数据
        for(int i = 0;i<num;i++){//一共有num个表的字段，每一个都和numstr个命令字段匹配
            for(int j =0;j<numstr;j++){
                if(strcmp(field[j],FieldSet[i].sFieldName)==0){
                //如果两个字符串相等，则把对应的命令存放到对应的待插入值里
                strcpy(forinsertvalue[i],values[j]);
                }
            }
        }
        /*按照表中给出的数据开始第一遍的检索，如果飞空的地方出现空值，则说明命令有问题，直接退出insert函数*/
        for(int i = 0;i<num;i++){
            if(FieldSet[i].bNullFlag=='n'&&strlen(forinsertvalue[i])==0){
                printf("对不起，您输入的命令不合法（非空字段为空），无法插入\n");
                return ;
            }
        }//以上处理全部完成，等待检查的数据页完成预设处理之后，开始打开文件，转码插入文件
        //开始写入对应的dat文件
        //创建对应的表，连接得到表名
        strcpy(dat,tname);
        strcat(dat,".dat");
        //printf("%s\n",dat);//检查表名是否连接合格
        dp = fopen(dat, "ab+");
        //写入格式化，为了方便查询
        fwrite("#",sizeof(char), 1, dp);
        for( int i = 0; i<num ;i++){
            if(strlen(forinsertvalue[i])==0){
                fwrite("NULL",sizeof(char),4,dp);   
            }
            else if(strlen(forinsertvalue[i])!=0){
                if (strcmp(FieldSet[i].sType,"int")==0){
                    int x = atoi(forinsertvalue[i]);
                    fwrite(&x,sizeof(int),1,dp);
                }
                else if(strcmp(FieldSet[i].sType,"double")==0){
                    double x =atof(forinsertvalue[i]);
                    fwrite(&x,sizeof(double),1,dp);
                }
                else{
                    fwrite(forinsertvalue[i],sizeof(char),FieldSet[i].iSize,dp);
                }
                
            }

        }
        fclose(dp);  
    }
    printf("Insert into %s %s successfully!\n",name,value);   
    }
    
}

/**删除函数，来对指定数据进行删除*/
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
   /**判断数据库和表是否打开*/
   if(dopens == 0){
        printf("数据库未打开，无法执行删除操作!\n");
    }
    else if(topens == 0){
        printf("表未打开，无法执行删除操作！\n");
    }
    else//检测有效，可以开始获取指令并录入信息
    {
    FILE *tp=NULL;//用来创建指向的临时文件
    FILE *dpt=NULL;//用来创建指向原文件待检测数据的指针
    char tname[FIELD_NAME_LENGTH]={0};//用来存放表名
    char tvalue[FIELD_NAME_LENGTH]={0};//用来暂时存放读取的数据
   
    char tflag;//用来存放读到的标志位
    //获得表名，字段数目
    strcpy(tname,tableName);
    TableMode FieldSet[MAX_SIZE];
    int num = OpenTable(tname,FieldSet);
    if (num == -1){
        printf("No such table!\n");
        return ;
    }
    else if(num == 0){
        printf("No field in table!\n");
        return ;
    }
    else{
    strcat(tname,".dat");
    //打开对应的文件
    dp = fopen(tname,"rb+");
    dpt = fopen(tname,"rb+");
    int databyte = 0;//不计算开始的标志位
    int desbyte = 0;//不计算开始的标志位
    //按照数据类型获得总共的字节数
    for(int i =0; i<num ; i++){
        if(strcmp(FieldSet[i].sType,"int")==0){
            databyte = databyte + sizeof(int);
        }
        else if(strcmp(FieldSet[i].sType,"double")==0){
            databyte =databyte + sizeof(double);
        }
        else{
            databyte = databyte + FieldSet[i].iSize;
        }  
    }
    //用来标志等待修改的列的位置
    int flag = 0;
    for(flag = 0;flag < num;flag++){
        if(strcmp(colName,FieldSet[flag].sFieldName)==0)//字符串相等
        break;
    }
    //获取待检测数据所在位置的字节数
    for(int i =0; i<flag ; i++){
        if(strcmp(FieldSet[i].sType,"int")==0){
            desbyte = desbyte + sizeof(int);
        }
        else if(strcmp(FieldSet[i].sType,"double")==0){
            desbyte =desbyte + sizeof(double);
        }
        else{
            desbyte = desbyte + FieldSet[i].iSize;
        }    
    }
    int it = 0;
    //读取当前文件中的内容，并且处理文件结果
    char ch ;
    while(!feof(dp)){
        int ml;
        ml = fread(&ch,sizeof(char),1,dp);
        if(!ml) break;//判断读取的值是否是格式化的有效

        tp = fopen("temp.dat","ab+");
        fseek(dpt,long(desbyte+1),SEEK_CUR);//相对于当前读到待检测数据；

        int seekflag = 0;//0代表要写入temp 文件，1不写
        int y = 0;//指示代检测的数据读取之后字节数
        //判断待删除的字段的数据类型
        if(strcmp(FieldSet[flag].sType,"int")==0){
            int x;
            y = sizeof(int);
            fread(&x,sizeof(int),1,dpt);
            if(x==atoi(value)){
                seekflag = 1;
            }
        }
        else if(strcmp(FieldSet[flag].sType,"double")==0){
            double x;
            y = sizeof(double);
            fread(&x,sizeof(double),1,dpt);
            if(fabs(x-atof(value))<1e-6){
                seekflag = 1;
            }
        }
        else{
            char x[50]={};
            y = FieldSet[flag].iSize;
            fread(x,sizeof(char),FieldSet[flag].iSize,dpt);
            if(strcmp(x,value)==0){
                seekflag = 1;
            }
        }
        //判断是否是all，如果是all直接全部删除
        if(strcmp(value,"all")==0){
            seekflag = 1;
        }
        if(seekflag == 0){
            char dataforinsert[2000];
            fwrite(&ch,sizeof(char),1,tp);
            fread(dataforinsert,sizeof(char),databyte,dp);
            fwrite(dataforinsert,sizeof(char),databyte,tp);
        }
        else{
            fseek(dp,long(databyte),SEEK_CUR);
        }
        fseek(dpt,long(databyte-desbyte-y),SEEK_CUR);
        it++;
            
        fclose(tp);
    }
    fclose(dpt);
    fclose(dp);
    remove(tname);
    rename("temp.dat",tname);

    printf("delete from %s where %s = %s successfully!\n",tableName,colName,value);
    }
    }
    
}
/**更新函数，对指定数据进行更新操作*/
void update(char* newColName ,char* newValue,char* tableName,char* colName,char* value){
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
    /**判断是否打开数据库和表文件*/
    if(dopens == 0){
        printf("数据库未打开，无法执行删除操作!\n");
    }
    else if(topens == 0){
        printf("表未打开，无法执行删除操作！\n");
    }
    else//检测有效，可以开始获取指令并录入信息
    {
    FILE *tp;//用来创建指向的临时文件
    FILE *dpt;//指向待检测数据；
    char tname[MAX_SIZE]={0};//用来存放表名
    int databyte = 0;//获得一条数据的总字节数
    int desbyte = 0;//获得待检测处数据前的总字节数
    int changebyte = 0;//获得待修改数据前的总字节数
    //复制表名，添加表名后缀
    strcpy(tname,tableName);
    TableMode FieldSet[MAX_SIZE];
    int num = OpenTable(tname,FieldSet);
    //得到一条数据的总字节数
    for(int i =0; i<num ; i++){
        if(strcmp(FieldSet[i].sType,"int")==0){
            databyte = databyte + sizeof(int);
        }
        else if(strcmp(FieldSet[i].sType,"double")==0){
            databyte =databyte + sizeof(double);
        }
        else{
            databyte = databyte + FieldSet[i].iSize;
        }   
    }
    //找到待检测的数据的位置
    int flag = 0;
    for(flag = 0;flag < num;flag++){
        if(strcmp(colName,FieldSet[flag].sFieldName)==0)//字符串相等
        break;
    }
    //获取待检测数据所在位置的字节数
    for(int i =0; i<flag ; i++){
        if(strcmp(FieldSet[i].sType,"int")==0){
            desbyte = desbyte + sizeof(int);
        }
        else if(strcmp(FieldSet[i].sType,"double")==0){
            desbyte =desbyte + sizeof(double);
        }
        else{
            desbyte = desbyte + FieldSet[i].iSize;
        }    
    }
    //找到待修改的数据
    int change = 0;
    for(change= 0;change < num;change++){
        if(strcmp(newColName,FieldSet[change].sFieldName)==0)//字符串相等
        break;
    }
    //获得待修改数据所在位置的字节数
    for(int i =0; i<change ; i++){
        if(strcmp(FieldSet[i].sType,"int")==0){
            changebyte = changebyte + sizeof(int);
        }
        else if(strcmp(FieldSet[i].sType,"double")==0){
            changebyte =changebyte + sizeof(double);
        }
        else{
            changebyte = changebyte + FieldSet[i].iSize;
        }    
    }
    strcat(tname,".dat");
    //打开文件并准备开始判断和修改操作
    dp = fopen(tname,"rb+");
    dpt = fopen(tname,"rb+");
    int it = 0;
    char ch ;
    while(!feof(dp)){
        int ml;
        ml = fread(&ch,sizeof(char),1,dp);
        if(!ml) break;
        tp = fopen("temp.dat","ab+");

        fseek(dpt,long(desbyte+1),SEEK_CUR);//找到待检测的数据的位置；
        int seekflag = 0;//0代表不修改写入temp 文件，1修改后写入
        int y = 0;//指示代检测的数据读取之后字节数
        //判断指示数据的类型，并根据字段类型读写
        if(strcmp(FieldSet[flag].sType,"int")==0){
            int x;
            y = sizeof(int);
            fread(&x,sizeof(int),1,dpt);
            if(x==atoi(value)){
                seekflag = 1;
            }
        }
        else if(strcmp(FieldSet[flag].sType,"double")==0){
            double x;
            y = sizeof(double);
            fread(&x,sizeof(double),1,dpt);;
            if(fabs(x-atof(value))<1e-6){
                seekflag = 1;
            }
        }
        else{
            char x[50]={};
            y = FieldSet[flag].iSize;
            fread(x,sizeof(char),FieldSet[flag].iSize,dpt);
            if(strcmp(x,value)==0){
                seekflag = 1;
            }
        }
        //如果字段指示为all,则对应位置的数据全部做修改
        if(strcmp(value,"all")==0){
            seekflag = 1;
        }
        if(seekflag == 0){
            
            char dataforinsert[2000];
            fwrite(&ch,sizeof(char),1,tp);
            fread(dataforinsert,sizeof(char),databyte,dp);
            fwrite(dataforinsert,sizeof(char),databyte,tp);
        }
        else{
            char dataforinsert[2000];
            fread(dataforinsert,sizeof(char),databyte,dp);//读取到待修改的数据，以二进制方式
            /**
             * 如果准备修改的是int类型的数据，
             * 现在得到的是字符串1
             * 读到的是int 1转码的二进制
             * 需要修改的是转成int之后再用二进制写入用char解释的码
             * 所以第一步是char转成int使用atoi函数
             * 第二步是int转成二进制，放到char数组
             * 第三步是在对应的位置转换char数组的内容
             * 
            */
           //判断待修改位置的数据类型，决定修改的方案
           if(strcmp(FieldSet[change].sType,"int")==0){
               int x;
               char yt[sizeof(int)];
               x = atoi(newValue);
               FILE *tmp;
               tmp = fopen("changedata","ab+");
               fwrite(&x,sizeof(int),1,tmp);
               fread(yt,sizeof(char),sizeof(int),tmp);
               fclose(tmp);
               remove("changedata");
               for(int i = changebyte;i<changebyte+sizeof(int);i++){
                   dataforinsert[i] = yt[i-changebyte];
               }
               fwrite(&ch,sizeof(char),1,tp);
               fwrite(dataforinsert,sizeof(char),databyte,tp);
           }
           else if(strcmp(FieldSet[flag].sType,"double")==0){
               double x;
               char yt[sizeof(double)];
               x = atof(newValue);
               FILE *tmp;
               tmp = fopen("changedata","ab+");
               fwrite(&x,sizeof(double),1,tmp);
               fread(yt,sizeof(char),sizeof(double),tmp);
               fclose(tmp);
               remove("changedata");
               for(int i = changebyte;i<changebyte+sizeof(double);i++){
                   dataforinsert[i] = yt[i-changebyte];
               }
               fwrite(&ch,sizeof(char),1,tp);
               fwrite(dataforinsert,sizeof(char),databyte,tp);
           }
           else{
               char yt[FILE_NAME_LENGTH] = {};
               strcpy(yt,newValue);
               for(int i = changebyte;i<changebyte+FieldSet[change].iSize;i++){//这里找一下怎么弄休止符号
                   if(i<changebyte+strlen(yt)){
                        dataforinsert[i] = yt[i-changebyte];
                   }
                   else{
                       dataforinsert[i] = '\0';
                   }
               }
               fwrite(&ch,sizeof(char),1,tp);
               fwrite(dataforinsert,sizeof(char),databyte,tp);
           }

        }
        fseek(dpt,long(databyte-desbyte-y),SEEK_CUR);
        it++; 

        fclose(tp);
    }
    fclose(dpt);
    fclose(dp);
    remove(tname);
    rename("temp.dat",tname);


    printf("update %s = %s from %s where %s = %s successfully!\n",tableName,newColName,newValue,colName,value);
    }
}