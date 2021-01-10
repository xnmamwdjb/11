#ifndef __DDL_H__ 
#define __DDL_H__ 

void CreateDataBase(char *name);
void OpenDataBase(char *name);
void CloseDataBase(char *name);
void DropDataBase(char *name);
void CreateTable(char *name);
void DropTable(char *name);
void OpenTable(char *name);
int execmd(char *cmd, char *result); 
#endif 