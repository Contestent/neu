#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
char path[1024];//Ҫ���ҵ�·���� 
char name[128];//Ҫ���ҵ��ļ��� 
void myls(char *path,char *name){
	char apath[1024];	//�洢����·��
	char newpath[1024]; //�ݹ�ʱ�洢��ʱ��·�� 
	DIR *dir;           //ǧ������Ϊȫ�ֱ���
	struct dirent *ptr;
	dir=opendir(path);  //���ݴ��������ָ��·�� 
	if(dir!=NULL){//���ڸ�Ŀ¼ʱ 
	while((ptr=readdir(dir))!=NULL){
		if(ptr->d_type==4/*DT_DIR*/&&strcmp(ptr->d_name,".")!=0&&strcmp(ptr->d_name,"..")!=0){ //�ݹ������ȱ���Ŀ¼ 
		    if(strcmp(ptr->d_name,name)==0){//����ҵ���ƥ��ļ�¼,��ƴ�Ӳ���ӡ·�� 
		        strcpy(apath,path);               //����ֱ�ӻ�ȡ����·��,��·��path���ݿ�������ʱ·��apath�� 
		        if(apath[strlen(apath)-1]=='/'){   //�жϵ�ַ��ʽ,�����β��/��ֱ������ļ������� 
		          strcat(apath,ptr->d_name);
		        }else{//�����β����/�����/�ָ�����׷���ļ��� 
		          strcat(apath,"/");
		          strcat(apath,ptr->d_name);
		        }
			  	printf("%s\n",apath);               //��ӡ���Ե�ַ
		    }
			//���û�ж�Ӧ�ļ���ƴ��·�������ڵݹ���� 
			if(path[strlen(path)-1]=='/'){
				sprintf(newpath,"%s%s",path,ptr->d_name);//·����ƴ�� 
			}else{
				sprintf(newpath,"%s/%s",path,ptr->d_name);
			}
			myls(newpath,name);  //�ݹ�ѭ������ 
		}else if(strcmp(ptr->d_name,".")!=0&&strcmp(ptr->d_name,"..")!=0&&strcmp(ptr->d_name,name)==0){  //�ҳ�ͬ���ļ�
			strcpy(apath,path);
			if(apath[strlen(apath)-1]=='/'){//·��ƴ�� 
				strcat(apath,ptr->d_name);
			}else{
				sprintf(apath,"%s/%s",apath,ptr->d_name);
			}
			printf("%s\n",apath);//��ӡ����·�� 
		}
	}//while���� 
	closedir(dir);//�ر�Ŀ¼��Դ 
	}//����Ŀ¼���жϷ�֧���� 
	else{//���Ŀ¼���ܷ���,����ʾ���� 
		printf("opendir error %s, need permission!\n",path);
	}
}//�Զ���ĺ������� 
int main(void){
	while(1){
		printf("path:");       //����Ŀ¼
		scanf("%s",path);      //�ļ�����
		printf("file:");
		scanf("%s",name);
		myls(path,name);
	}
}
