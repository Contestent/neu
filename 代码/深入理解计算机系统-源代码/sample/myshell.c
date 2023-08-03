#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include <wait.h>

#define MAXLINE 1024 //����ж�ȡ�� 
#define MAXARGS 128	//����������,linux���֧��128K,POSIX���֧��4096[�ҵļ�����������,ʵ��ʱ�Լ�������] 

void eval(char *cmdline);//ִ�� 
int parseline(char *buf, char **argv);//���������� 
int builtin_command(char **argv);//�������� 

int main(int argc,char *argv)
{
        char cmdline[MAXLINE];//�û������û����� 
        while(1){//����һ����ѭ��,ֻ���յ��źŻ�������quitʱ�˳� 
                printf("myshell> ");//��ʾ���������Ҫ����ʹ��ϵͳ���û���û����Ƶ���Ϣ��� 
                fgets(cmdline,MAXLINE,stdin);//�ӱ�׼�����豸���� 
                if(feof(stdin)) 
                        exit(EXIT_FAILURE);
                eval(cmdline);//ִ�ж�������� 
        }
}

void eval(char *cmdline)

{
        char * argv[MAXARGS];//���ֻ����128������,�������ǹ��˵� 
        char buf[MAXLINE];//��������� 
        int bg;//�Ƿ��Ǻ�̨��ҵ   1 ��   0 ���� 
        pid_t pid;//�����ӽ��̵Ľ��̱�� 

        strcpy(buf,cmdline);//���ǵ���˵��,����Դ��ݹ������ַ�������,�ǵÿ���������Ŷ 
        bg = parseline(buf,argv);//������������Ƿ��Ǻ�̨��ҵ 
        if(argv[0] == NULL)//��������ڵ�һ������,��ζ��ֱ�������˿���,�����ѭ����ʾ��ʾ�� 
                return;
        if(!builtin_command(argv)){//�������quit(����0),�������Ҫִ����Ӧ���� 
                if((pid = fork()) == 0){//����һ���ӽ�������ִ��ҵ���߼� 
                        if(execvp(argv[0],argv) < 0){//�˴�����ѡ��execve�������غ���,����Ϊ�˼��ʹ���˴����������ĵ���execvp 
                                printf("%s: Command not found. \n",argv[0]); 
                                exit(0);
                        }
                }

                if(!bg){//������Ǻ�̨���� 
                        int status;
                        if(waitpid(pid,&status,0) < 0)//����������,�ȴ������ӽ��� 
                                perror("waitpid");
                }else{//��̨������ʾ 
                        printf("%d %s",pid,cmdline);
                }
        }
        return;

}
int parseline(char *buf, char **argv)
{
        char *delimit;//����ָ��� 
        int argc;//�������� 
        int bg;//�Ƿ��Ǻ�̨���� 
        buf[strlen(buf)-1] = ' ';//�����һ�����������滻Ϊ �ո�  ,ȷ���س�+���� �����ܹ���ȷ���� 
        while(*buf && (*buf == ' ')){// ����ǰ�õĿո��ַ� 
                buf++;
        }
        argc = 0;
        while((delimit = strchr(buf,' '))){//�����ַ���,���ص�һ��ƥ��ĵط�,������ƥ��ո�Ϊ�ָ��� 
                argv[argc++] = buf; 
                *delimit = '\0';
                buf = delimit + 1;
                while(*buf && (*buf == ' ')){//���Կո� 
                        buf++;
                }
        }
        argv[argc] = NULL;//����ڱ�,��ζ������������� 
        if(argc == 0)//���Կ������� 
                return 1;
        if((bg = (*argv[argc-1] == '&')) != 0)//������һ�������� & �����Ǻ�̨��ҵ 
                argv[--argc] = NULL;//����ڱ� 

        return bg;
}
int builtin_command(char **argv)
{
        if(!strcmp(argv[0],"quit"))//�˳����� 
                exit(0);
        if(!strcmp(argv[0],"&"))//  
                return 1;
        return 0;
}

