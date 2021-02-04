#include "head.h"

#define SPLICESIZE  8192

int recvCycle(int fd,void* buf,size_t len);

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    //连接服务器，等待传输文件
    //先创建套接字,用于跟服务端通信
    int serverFd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(serverFd,-1,"socket");

    //定义结构体，记录ip和端口
    struct sockaddr_in serAddr;
    memset(&serAddr,0,sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    //把字符串的点分十进制ip转换成网络字节序的二进制ip
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serAddr.sin_port = htons(atoi(argv[2]));
 
    //connect连接服务器，结构体中填写的是服务器的ip和端口
    int ret = connect(serverFd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    ERROR_CHECK(ret,-1,"connect");


    char buf[1000]={0};
    
    //1.接收文件名
    //TCP传输的数据是流式的数据
    //消息和消息之间没有边界
    
    //接收数据时，先接受火车头
    //这里的火车头记录了文件名的长度
    int dataLen = 0;
    recv(serverFd,&dataLen,sizeof(int),0);
    //接车厢中数据的时候,按照火车头中记录的长度去接
    //这样可以不多不少，刚好接到想要的数据
    recv(serverFd,buf,dataLen,0);

    int fd = open(buf,O_RDWR|O_CREAT,0666);
    ERROR_CHECK(fd,-1,"open");
    printf("recv buf=%s\n",buf);

    //接收文件大小，用于打印进度条
    off_t fileSize = 0;
    recv(serverFd,&dataLen,sizeof(int),0);
    recv(serverFd,&fileSize,dataLen,0);
    printf("fileSize=%ld\n",fileSize);

    //把文件设置位特定的大小
    ftruncate(fd,fileSize);
    struct timeval start,end;
    memset(&start,0,sizeof(start));
    memset(&end,0,sizeof(start));
    gettimeofday(&start,NULL);

    off_t downSize = 0;
    //记录上次打印进度条时的大小
    float scale=0.02;
    size_t slice=4096;

    //2.接收文件内容，并且写入到文件中
    //先接受一下文件内容的长度，也就是文件内容有多少字节
    //再按照长度去接后面的文件内容
    /* close(serverFd); */

    char* pMap = (char*)mmap(NULL,fileSize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(pMap,(char*)-1,"mmap");

    //为了能打印进度条，拆分recv
    while(downSize<fileSize)
    {
        int ret=recv(serverFd,pMap+downSize,slice,0);
        downSize += ret;
        if((float)downSize/fileSize>scale)
        {
            for(int i=scale/0.02;i>0;i--)
                printf("-");
            printf("%5.2f%s\r",(float)downSize*100/fileSize,"%");
            fflush(stdout);
            scale+=0.02;
        }
    }

    printf("\n");
    gettimeofday(&end,NULL);

    printf("cost time=%ld\n",(end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec);
    munmap(pMap,fileSize);
    close(serverFd);
    return 0;
}
