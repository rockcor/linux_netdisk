#include "../include/head.h"
#include "../include/cmd.h"


int main()
{
    char buf[1024]={0};
    char pwd[128]={0};
    int ret=0;

    while(1)
    {
        net_pwd();
        getcwd(pwd,sizeof(pwd));
        memset(buf,0,sizeof(buf));
        read(STDIN_FILENO,buf,sizeof(buf));
        if(strcmp(buf,"pwd\n") == 0)
        {
            printf("%s\n",pwd);
        }
        else if(strcmp(buf,"\n") == 0);
        else if(strncmp(buf,"cd",2) == 0)
        {
            buf[strlen(buf)-1]='\0';
            //只能打一个空格
            sprintf(pwd,"%s/%s",pwd,buf+3);
            ret=chdir(pwd);
            if(-1==ret)
            {
                printf("No such file or directory\n");
            }
        }
        else if(strncmp(buf,"ls",2) == 0)
        {
            myls(pwd);
        }
        else if(strncmp(buf,"puts",4) == 0)
        {
            /* buf[strlen(buf)-1]='\0'; */
            /* ("./rec","recfile",NULL); */
        }
        else if(strncmp(buf,"gets",4) == 0)
        {
            /* buf[strlen(buf)-1]='\0'; */
            /* ("./send","sendfile",buf+5,NULL); */
        }
        else if(strncmp(buf,"remove",6) == 0)
        {
            buf[strlen(buf)-1]='\0';
            //只能打一个空格
            sprintf(pwd,"%s/%s",pwd,buf+7);
            remove(pwd);
            if(!ret)
            {
                printf("Delete failed\n");
            }
        }
        else{
            printf("error cmd\n");
        }
    }
}
