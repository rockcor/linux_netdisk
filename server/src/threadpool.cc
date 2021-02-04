#include "../include/threadpool.h"

void* threadFunc(void* pArg)
{
    pThreadpool_t pPool =(pThreadpool_t)pArg;
    pQue_t pQue=&pPool->que;
    pNode_t pCur=NULL;
    while(1)
    {
        pthread_mutex_lock(&pQue->mlock);
        if(0==pQue->size)
        {
            printf("child wait\n");
            pthread_cond_wait(&pQue->cond,&pQue->mlock);
            printf("child wake up\n");
        }
        int suc=queGet(pQue,&pCur);
        pthread_mutex_unlock(&pQue->mlock);
        if(0==suc)
        {
            transFile(pCur->clientFd);
            close(pCur->clientFd);
        }
    }
}

int threadPoolInit(pThreadpool_t pPool,int threadnum)
{
    memset(pPool,0,sizeof(threadpool_t));
    //对结构体中所有成员附一个初值
    queInit(&pPool->que);
    pPool->threadnum = threadnum;
    pPool->startFlag = 0;

    //为保存线程id的数组分配堆空间
    pPool->pThid =(pthread_t*) calloc(threadnum,sizeof(pthread_t));

    return 0;
}

int threadPoolStart(pThreadpool_t pPool)
{
    if(0 == pPool->startFlag)
    {
        //循环创建子线程
        for(int i=0;i<pPool->threadnum;i++)
        {
            /* pthread_create(&pPool->pThid[i],NULL,threadFunc,pPool); */
            pthread_create(pPool->pThid+i,NULL,threadFunc,pPool);
        }
    }

    pPool->startFlag = 1;
    return 0;
}
