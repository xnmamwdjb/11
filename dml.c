#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
//#include<sstream>

extern int OpenTable(char *name, PTableMode FieldSet);

//�ָ��ַ�������������ʵ�ֶ�ָ���ַ��ķָ�
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
    //for(int i = 0;i<strlen(name);i++){
        //str[i] = name[i];
    //}
    /* for(int i=strlen(name);i<100;i++){
        str[i] = '\0';
    } */
    //printf("%s\n",str);//��������Ƿ�ֵ�ϸ�
    //��һ�ַָ�ָ������
    split(str,"(",tempname,&numstr);
    for(int i = 0; i<strlen(tempname[0]);i++){
        tname[i] = tempname[0][i]; 
    }
    strcpy(str,tempname[1]);
    /* for(int i = 0; i<strlen(tempname[1]); i++){
        str[i] = tempname[1][i]; 
    } */
    //str[strlen(tempname[1])] = '\0';
    //printf("%s\n",str);//��������ʵ�ֵ�����ָ��Ƿ�ϸ�
    //�ڶ��ַָ�ָ�������fieldֵ
    split(str,")",tempname,&numstr);
    strcpy(str,tempname[0]);
    /* for(int i = 0; i<strlen(tempname[0]);i++){
        str[i] = tempname[0][i];
    }
    str[strlen(tempname[0])] = '\0'; */
    //printf("%s\n",str);//��������ʵ�ֵ�����ָ��Ƿ�ϸ�
    //�����ַָ�ָ�������ֶε�ֵ����֤д��
    split1(str,",",field,&numstr);
    //����������շָ��ֶ����Ƿ�ϸ�
    /*
    for(int i = 0 ;i<numstr;i++){
        printf("%s\n",field[i]);
    }
    */
    //��values�ֶε�ֵ��ֵ��str
    /* for(int i = 0;i<strlen(value);i++){
        str[i] = value[i];
    } */
    strcpy(str,value);
    /* for(int i=strlen(name);i<100;i++){
        str[i] = '\0';
    } */
    //printf("%s\n",str);//��������Ƿ�ֵ�ϸ�
    //��һ�ַָ�ָ����values�ؼ���
    split(str,"(",tempname,&numstr);
    strcpy(str,tempname[1]);
    /* for(int i = 0; i<strlen(tempname[1]); i++){
        str[i] = tempname[1][i]; 
    } */
    /* for(int i = strlen(tempname[1]);i<100;i++){
        str[i] = '\0';
    } */
    //printf("%s\n",str);//��������ʵ�ֵ�����ָ��Ƿ�ϸ�
    //�ڶ��ַָ�ָ�������valueֵ
    split(str,")",tempname,&numstr);
    /* for(int i = 0; i<strlen(tempname[0]);i++){
        str[i] = tempname[0][i];
    }
    str[strlen(tempname[0])] = '\0'; */
    strcpy(str,tempname[0]);
    //printf("%s\n",str);//��������ʵ�ֵ�����ָ��Ƿ�ϸ�
    //�����ַָ�ָ�������ֶε�ֵ����֤д��
    split1(str,",",values,&numstr);
    /* for(int i = 0 ;i<numstr;i++){
        printf("%s\n",values[i]);
    } */

    /* for(int i = 0;i<numstr;i++){
        values[i][strlen(values[i])] = '\40';
        for(int j = strlen(values[i])+1;j<FIELD_NAME_LENGTH;j++){
            values[i][j]='\40';
        }
    } */
    //����������շָ��ֶ����Ƿ�ϸ�
    /*
    for(int i = 0 ;i<numstr;i++){
        printf("%s\n",values[i]);
        //printf("%p\n",values[i]);//ָʾָ���ַ������ָ���ַ֮��Ϊ2��˵����������
    }
    */
    
   /*��ñ�ṹ�б�ֵ*/
    TableMode FieldSet[MAX_SIZE];
    int num = OpenTable(tname, FieldSet);//�ֲ������������õ�����ֶθ���ֵ
    if (num == -1)
        printf("No such table!\n");
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

        fwrite("#",sizeof(char), 1, dp);
        for( int i = 0; i<num ;i++){
            if(strlen(forinsertvalue[i])==0){
                fwrite("NULL",sizeof(char),4,dp);   
            }
            else if(strlen(forinsertvalue[i])!=0){
                if (strcmp(FieldSet[i].sType,"int")==0){
                    int x = atoi(forinsertvalue[i]);
                    //printf("%d",x);
                    fwrite(&x,sizeof(int),1,dp);
                }
                else if(strcmp(FieldSet[i].sType,"double")==0){
                    double x =atof(forinsertvalue[i]);
                    printf("%lf",x);
                    fwrite(&x,sizeof(double),1,dp);
                }
                else{
                    fwrite(forinsertvalue[i],sizeof(char),FieldSet[i].iSize,dp);
                }
                
            }
            //printf("%p\n",dp);
        }
        fclose(dp);  
    }
        
    }
    //printf("insert into %s %s\n",name,value);
}
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
    /* for(int i = 0 ;i<strlen(tableName);i++){
        tname[i] = tableName[i];
    } */
    strcpy(tname,tableName);
    
    //printf("%s",tname);
    

    TableMode FieldSet[MAX_SIZE];

    int num = OpenTable(tname,FieldSet);
    strcat(tname,".dat");
    //�򿪶�Ӧ���ļ�
    dp = fopen(tname,"rb+");
    dpt = fopen(tname,"rb+");
    int databyte = 0;//�������˿�ʼ�ı�־λ
    int desbyte = 0;//�������˿�ʼ�ı�־λ
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
        //printf("%d\n",databyte);  
    }
    //databyte = databyte +1;
    
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
    
    //desbyte = desbyte + 1;
    //printf("%d\n",databyte);
    //printf("%d\n",desbyte);
    int it = 0;
    //printf("%p\n",dpt);
    //printf("%s\n",value);
    //��ȡ��ǰ�ļ��е����ݣ����Ҵ����ļ����
    char ch ;
    while(!feof(dp)){
        int ml;
        ml = fread(&ch,sizeof(char),1,dp);
        //printf("%c",ch);
        if(!ml) break;

        //printf("%d\n",it);
        
        tp = fopen("temp.dat","ab+");
        fseek(dpt,long(desbyte+1),SEEK_CUR);//����ڵ�ǰ������������ݣ�
        //printf("%d\n",xxx);
        //printf("%p\n",dp);
        //printf("%p\n",dpt);
        int seekflag = 0;//0����Ҫд��temp �ļ���1��д
        int y = 0;//ָʾ���������ݶ�ȡ֮���ֽ���

        if(strcmp(FieldSet[flag].sType,"int")==0){
            int x;
            y = sizeof(int);
            fread(&x,sizeof(int),1,dpt);
            //printf("zhe int shuchu\n");
            if(x==atoi(value)){
                seekflag = 1;
            }
        }
        else if(strcmp(FieldSet[flag].sType,"double")==0){
            double x;
            y = sizeof(double);
            fread(&x,sizeof(double),1,dpt);
            printf("%f\n",x);
            //printf("zhe double shuchu\n");
            if(fabs(x-atof(value))<1e-6){
                seekflag = 1;
            }
        }
        else{
            char x[50]={};
            //printf("%s\n",x);
            y = FieldSet[flag].iSize;
            fread(x,sizeof(char),FieldSet[flag].iSize,dpt);
            //printf("zhe char shuchu\n");
            //printf("%s\n",x);
            if(strcmp(x,value)==0){
                seekflag = 1;
            }
        }
        if(seekflag == 0){
            
            char dataforinsert[2000];
            //printf("%s",dataforinsert);
            fwrite(&ch,sizeof(char),1,tp);
            fread(dataforinsert,sizeof(char),databyte,dp);
            fwrite(dataforinsert,sizeof(char),databyte,tp);
        }
        else{
            fseek(dp,long(databyte),SEEK_CUR);
        }
        //fseek(dp,databyte,SEEK_CUR);
        fseek(dpt,long(databyte-desbyte-y),SEEK_CUR);
        printf("%d\n",databyte);
        printf("%d\n",desbyte);
        printf("%d\n",y);
        printf("%d\n",databyte-desbyte-y);
        printf("\n");
        it++;
            
        fclose(tp);
    }
    fclose(dpt);
    fclose(dp);
    remove(tname);
    rename("temp.dat",tname);
    }
    //printf("delete from %s where %s = %s\n",tableName,colName,value);
}
void update(char* tableName,char* newColName,char* newValue,char* colName,char* value){
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
    FILE *tp;//��������ָ�����ʱ�ļ�
    char tname[MAX_SIZE]={0};//������ű���
    //���Ʊ�������ӱ�����׺
    for(int i = 0 ;i<strlen(tableName);i++){
        tname[i] = tableName[i];
    }
    strcat(tname,".dat");
    //���ļ���׼����ʼ�жϺ��޸Ĳ���
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