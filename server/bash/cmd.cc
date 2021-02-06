#include "../include/head.h"
#include "../include/cmd.h"

void net_pwd()
{
    char *pwd=getcwd(NULL,0);
    printf("%s%s",pwd,"  netdisk>");
    fflush(stdout);
}

void myls(const char *path)
{
    DIR *dir=opendir(path);
    if(dir==NULL)
    {
        perror("opendir");
        return ;
    }
    struct dirent *p;
    while(p=readdir(dir))
    {
        if(!strcmp(p->d_name,".")||!strcmp(p->d_name,".."))
        {continue;}
        printf("%s\n",p->d_name);
    }
    closedir(dir);
    return;
}
