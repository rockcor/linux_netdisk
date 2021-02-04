#include "../include/threadpool.h"

#define FILENAME "file"


int transFile(int clientFd)
{
    //所有要发送的数据，先装入小火车
    //再通过小火车发送
    //1.打开文件
    int fd = open(FILENAME,O_RDWR);
    ERROR_CHECK(fd,-1,"open");

    int ret = 0;
    //2.发送文件名
    train_t train;
    int headLen=sizeof(train.dataLen);
    memset(&train,0,sizeof(train));
    strcpy(train.buf,FILENAME);
    train.dataLen = strlen(FILENAME);
    ret = send(clientFd,&train,headLen+train.dataLen,0);
    ERROR_CHECK(ret,-1,"sendname");


    //发送文件大小，可以用于客户端退出循环接收
    //可以用于客户端打印进度条
    struct stat filestat;
    memset(&filestat,0,sizeof(filestat));
    ret = fstat(fd,&filestat);
    ERROR_CHECK(ret,-1,"fstat");
    memcpy(train.buf,&filestat.st_size,sizeof(filestat.st_size));
    train.dataLen = sizeof(filestat.st_size);
    ret = send(clientFd,&train,headLen+train.dataLen,0);
    ERROR_CHECK(ret,-1,"sendname");

    //3.读文件内容并发送，大火车
    char* pMap = (char*)mmap(NULL,filestat.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(pMap,(char*)-1,"mmap");

    ret = send(clientFd,pMap,filestat.st_size,0);
    ERROR_CHECK(ret,-1,"send");

    munmap(pMap,filestat.st_size);

    close(fd);
    return 0;
}
