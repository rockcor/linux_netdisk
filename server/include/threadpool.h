#pragma once
#include "head.h"
#include "workque.h"

//定义线程池结构体

typedef struct{
    que_t que;
    int threadnum;
    pthread_t *pThid;
    short startFlag;//线程池启动标志
}threadpool_t,*pThreadpool_t;

typedef struct{
    int dataLen;
    char buf[1000];
}train_t;

int threadPoolInit(pThreadpool_t pPool,int threadnum);
int threadPoolStart(pThreadpool_t);

int tcpInit(int* socketFd,char* ip,char* port);
int epollAdd(int epfd,int fd);
int transFile(int clientFd);

