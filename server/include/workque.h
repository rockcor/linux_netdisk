
#pragma once
#include "head.h"

//队列的任务节点，记录客户端的套接字描述符
typedef struct node{
    int clientFd;
    struct node* pNext;
}node_t,*pNode_t;

typedef struct{
    pNode_t pHead,pTail;
    int size;//队列大小
    pthread_mutex_t mlock;
    pthread_cond_t cond;
}que_t,*pQue_t;

int queInit(pQue_t pQue);
int queInsert(pQue_t pQue,pNode_t pNew);
int queGet(pQue_t pQue,pNode_t* pGet);

