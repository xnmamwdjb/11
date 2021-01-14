#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

extern int OpenTable(char *name, PTableMode FieldSet);

/**�ָ��ַ�������������ʵ�ֶ�ָ���ַ��ķָ�,ָ������*/
void split(char *src,const char *separator,char **dest,int *numstr) {
	/*
		src Դ�ַ������׵�ַ(buf�ĵ�ַ) 
		separator ָ���ķָ��ַ�
		dest �������ַ���������
		numstr �ָ�����ַ����ĸ���
	*/
     char *pNext;
     int count = 0;
     if (src == NULL || strlen(src) == 0) //�������ĵ�ַΪ�ջ򳤶�Ϊ0��ֱ����ֹ 
        return;
     if (separator == NULL || strlen(separator) == 0) //��δָ���ָ���ַ�����ֱ����ֹ 
        return;
     pNext = (char *)strtok(src,separator); //����ʹ��(char *)����ǿ������ת��(��Ȼ��д�еı������в������ָ�����)
     while(pNext != NULL) {
          *dest++ = pNext;
          ++count;
         pNext = (char *)strtok(NULL,separator);  //����ʹ��(char *)����ǿ������ת��
    }  
    *numstr = count;
} 	

/**�ָ��ַ�����������������*/
void split1(char *src,const char *separator,char dest[50][15],int *numstr) {
	/*
		src Դ�ַ������׵�ַ(buf�ĵ�ַ) 
		separator ָ���ķָ��ַ�
		dest �������ַ���������
		numstr �ָ�����ַ����ĸ���
	*/
     char *pNext;
     int count = 0;
     if (src == NULL || strlen(src) == 0) //�������ĵ�ַΪ�ջ򳤶�Ϊ0��ֱ����ֹ 
        return;
     if (separator == NULL || strlen(separator) == 0) //��δָ���ָ���ַ�����ֱ����ֹ 
        return;
     pNext = (char *)strtok(src,separator); //����ʹ��(char *)����ǿ������ת��(��Ȼ��д�еı������в������ָ�����)
     int i = 0;
     while(pNext != NULL) {
        for(int j = 0;j<strlen(pNext);j++){
            dest[i][j] = pNext[j];
        }
        dest[i][strlen(pNext)]='\0';
          ++count;
         pNext = (char *)strtok(NULL,separator);  //����ʹ��(char *)����ǿ������ת��
         i++;
    }  
    *numstr = count;
} 	

/**���뺯����ʵ�ֶ����ݰ��ձ�ṹ�Ĳ��빦��*/
void Insert(char* name,char* value){
    //name�ֶΰ����˱������ֶ�����...
    //value�ֶΰ�����values���ֶ�ֵ��...
    /*��һ��������Ƿ�����ݿ�ͱ����δ�򿪣��򱨴�*/
    if(dopens == 0){
        printf("���ݿ�δ�򿪣��޷�ִ�в������!\n");
    }
    else if(topens == 0){
        printf("��δ�򿪣��޷�ִ�в��������\n");
    }
    else//�����Ч�����Կ�ʼ��ȡָ�¼����Ϣ
    {
    /*�ڶ�������������ֶβ��ָ������ֶ�*/
    int numstr = 0;//�����õ������ֶηָ�֮����ܸ���
    char tname[MAX_SIZE]={0};//������ŷָ�֮��ı���
    char *tempname[MAX_SIZE]={0};//��������м��ֶ��Խ����ٷ���
    char str[100]={0};//������������ֶΣ���ʵ���ٷָ�
    char field[50][15]={};//������ŷָ�֮����ֶ���[FIELD_NAME_LENGTH]
    char values[50][15]={};//������ŷָ�֮����ֶ�ֵ
    //��name�ֶε�ֵ��ֵ��str
    strcpy(str,name);
    //��һ�ַָ�ָ������
    split(str,"(",tempname,&numstr);
    for(int i = 0; i<strlen(tempname[0]);i++){
        tname[i] = tempname[0][i]; 
    }
    strcpy(str,tempname[1]);
    //�ڶ��ַָ�ָ�������fieldֵ
    split(str,")",tempname,&numstr);
    strcpy(str,tempname[0]);
    //�����ַָ�ָ�������ֶε�ֵ����֤д��
    split1(str,",",field,&numstr);
    //��values�ֶε�ֵ��ֵ��str
    strcpy(str,value);
    //��һ�ַָ�ָ����values�ؼ���
    split(str,"(",tempname,&numstr);
    strcpy(str,tempname[1]);
    //�ڶ��ַָ�ָ�������valueֵ
    split(str,")",tempname,&numstr);
    strcpy(str,tempname[0]);
    //�����ַָ�ָ�������ֶε�ֵ����֤д��
    split1(str,",",values,&numstr);
   /*��ñ�ṹ�б�ֵ*/
    TableMode FieldSet[MAX_SIZE];
    int num = OpenTable(tname, FieldSet);//�ֲ������������õ�����ֶθ���ֵ
    if (num == -1){
        printf("No such table!\n");
        return;
    }
    else if (num == 0)
        printf("No field in table!\n");
    else
    {
        /*����ղŵ�����ֵ��ʹ������ձ��˳�����Ԥ����*/
        char fieldname[MAX_SIZE][FIELD_NAME_LENGTH]={0};//������ű����ж�ȡ�õ����ֶ�����ʱƥ�䣬ԭ����������д���ʱ���ֶ�˳������������
        char forinsertvalue[MAX_SIZE][FIELD_NAME_LENGTH]={0};//��������˳���Ŵ����������
        for(int i = 0;i<num;i++){//һ����num������ֶΣ�ÿһ������numstr�������ֶ�ƥ��
            for(int j =0;j<numstr;j++){
                if(strcmp(field[j],FieldSet[i].sFieldName)==0){
                //��������ַ�����ȣ���Ѷ�Ӧ�������ŵ���Ӧ�Ĵ�����ֵ��
                strcpy(forinsertvalue[i],values[j]);
                }
            }
        }
        /*���ձ��и��������ݿ�ʼ��һ��ļ���������ɿյĵط����ֿ�ֵ����˵�����������⣬ֱ���˳�insert����*/
        for(int i = 0;i<num;i++){
            if(FieldSet[i].bNullFlag=='n'&&strlen(forinsertvalue[i])==0){
                printf("�Բ��������������Ϸ����ǿ��ֶ�Ϊ�գ����޷�����\n");
                return ;
            }
        }//���ϴ���ȫ����ɣ��ȴ���������ҳ���Ԥ�账��֮�󣬿�ʼ���ļ���ת������ļ�
        //��ʼд���Ӧ��dat�ļ�
        //������Ӧ�ı����ӵõ�����
        strcpy(dat,tname);
        strcat(dat,".dat");
        //printf("%s\n",dat);//�������Ƿ����Ӻϸ�
        dp = fopen(dat, "ab+");
        //д���ʽ����Ϊ�˷����ѯ
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

/**ɾ������������ָ�����ݽ���ɾ��*/
void Delete(char* tableName,char* colName,char* value){
    /**
     * Ϊ��ʵ��ɾ������
     * ��Ҫ�����Ƕ�ȡ�ļ���������Ϣ
     * Ȼ��ͨ���ַ����ȶԽ��д���
     * Ȼ������д�봦������������
     * ����Ҫ����һ����ʱ�ļ�
     * ��ȡԭ�ļ���һ�м�¼
     * �ж��Ƿ���Ҫɾ��
     * д����ʱ�ļ�
     * ȫ����ȡ��Ϲر�ԭ�ļ���ɾ��
     * ����ʱ�ļ�����Ϊԭ�ļ���
    */
   /**�ж����ݿ�ͱ��Ƿ��*/
   if(dopens == 0){
        printf("���ݿ�δ�򿪣��޷�ִ��ɾ������!\n");
    }
    else if(topens == 0){
        printf("��δ�򿪣��޷�ִ��ɾ��������\n");
    }
    else//�����Ч�����Կ�ʼ��ȡָ�¼����Ϣ
    {
    FILE *tp=NULL;//��������ָ�����ʱ�ļ�
    FILE *dpt=NULL;//��������ָ��ԭ�ļ���������ݵ�ָ��
    char tname[FIELD_NAME_LENGTH]={0};//������ű���
    char tvalue[FIELD_NAME_LENGTH]={0};//������ʱ��Ŷ�ȡ������
   
    char tflag;//������Ŷ����ı�־λ
    //��ñ������ֶ���Ŀ
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
    //�򿪶�Ӧ���ļ�
    dp = fopen(tname,"rb+");
    dpt = fopen(tname,"rb+");
    int databyte = 0;//�����㿪ʼ�ı�־λ
    int desbyte = 0;//�����㿪ʼ�ı�־λ
    //�����������ͻ���ܹ����ֽ���
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
    //������־�ȴ��޸ĵ��е�λ��
    int flag = 0;
    for(flag = 0;flag < num;flag++){
        if(strcmp(colName,FieldSet[flag].sFieldName)==0)//�ַ������
        break;
    }
    //��ȡ�������������λ�õ��ֽ���
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
    //��ȡ��ǰ�ļ��е����ݣ����Ҵ����ļ����
    char ch ;
    while(!feof(dp)){
        int ml;
        ml = fread(&ch,sizeof(char),1,dp);
        if(!ml) break;//�ж϶�ȡ��ֵ�Ƿ��Ǹ�ʽ������Ч

        tp = fopen("temp.dat","ab+");
        fseek(dpt,long(desbyte+1),SEEK_CUR);//����ڵ�ǰ������������ݣ�

        int seekflag = 0;//0����Ҫд��temp �ļ���1��д
        int y = 0;//ָʾ���������ݶ�ȡ֮���ֽ���
        //�жϴ�ɾ�����ֶε���������
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
        //�ж��Ƿ���all�������allֱ��ȫ��ɾ��
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
/**���º�������ָ�����ݽ��и��²���*/
void update(char* newColName ,char* newValue,char* tableName,char* colName,char* value){
    /**
     * Ϊ��ʵ���޸Ĳ���
     * ��Ҫ����һ����ʱ�ļ�
     * ��ԭ�ļ������ж�ȡ����
     * ���������������ݽ����޸�
     * ����������޸�������ֱ��д��
     * ��������޸������Ͱ��������޸ĺ�д��
     * д����ʱ�ļ�
     * ԭ�ļ�����֮��ر�ԭ�ļ���ɾ��
     * ����ʱ�ļ����Ƹ�Ϊԭ�ļ�����
    */
    /**�ж��Ƿ�����ݿ�ͱ��ļ�*/
    if(dopens == 0){
        printf("���ݿ�δ�򿪣��޷�ִ��ɾ������!\n");
    }
    else if(topens == 0){
        printf("��δ�򿪣��޷�ִ��ɾ��������\n");
    }
    else//�����Ч�����Կ�ʼ��ȡָ�¼����Ϣ
    {
    FILE *tp;//��������ָ�����ʱ�ļ�
    FILE *dpt;//ָ���������ݣ�
    char tname[MAX_SIZE]={0};//������ű���
    int databyte = 0;//���һ�����ݵ����ֽ���
    int desbyte = 0;//��ô���⴦����ǰ�����ֽ���
    int changebyte = 0;//��ô��޸�����ǰ�����ֽ���
    //���Ʊ�������ӱ�����׺
    strcpy(tname,tableName);
    TableMode FieldSet[MAX_SIZE];
    int num = OpenTable(tname,FieldSet);
    //�õ�һ�����ݵ����ֽ���
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
    //�ҵ����������ݵ�λ��
    int flag = 0;
    for(flag = 0;flag < num;flag++){
        if(strcmp(colName,FieldSet[flag].sFieldName)==0)//�ַ������
        break;
    }
    //��ȡ�������������λ�õ��ֽ���
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
    //�ҵ����޸ĵ�����
    int change = 0;
    for(change= 0;change < num;change++){
        if(strcmp(newColName,FieldSet[change].sFieldName)==0)//�ַ������
        break;
    }
    //��ô��޸���������λ�õ��ֽ���
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
    //���ļ���׼����ʼ�жϺ��޸Ĳ���
    dp = fopen(tname,"rb+");
    dpt = fopen(tname,"rb+");
    int it = 0;
    char ch ;
    while(!feof(dp)){
        int ml;
        ml = fread(&ch,sizeof(char),1,dp);
        if(!ml) break;
        tp = fopen("temp.dat","ab+");

        fseek(dpt,long(desbyte+1),SEEK_CUR);//�ҵ����������ݵ�λ�ã�
        int seekflag = 0;//0�����޸�д��temp �ļ���1�޸ĺ�д��
        int y = 0;//ָʾ���������ݶ�ȡ֮���ֽ���
        //�ж�ָʾ���ݵ����ͣ��������ֶ����Ͷ�д
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
        //����ֶ�ָʾΪall,���Ӧλ�õ�����ȫ�����޸�
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
            fread(dataforinsert,sizeof(char),databyte,dp);//��ȡ�����޸ĵ����ݣ��Զ����Ʒ�ʽ
            /**
             * ���׼���޸ĵ���int���͵����ݣ�
             * ���ڵõ������ַ���1
             * ��������int 1ת��Ķ�����
             * ��Ҫ�޸ĵ���ת��int֮�����ö�����д����char���͵���
             * ���Ե�һ����charת��intʹ��atoi����
             * �ڶ�����intת�ɶ����ƣ��ŵ�char����
             * ���������ڶ�Ӧ��λ��ת��char���������
             * 
            */
           //�жϴ��޸�λ�õ��������ͣ������޸ĵķ���
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
               for(int i = changebyte;i<changebyte+FieldSet[change].iSize;i++){//������һ����ôŪ��ֹ����
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