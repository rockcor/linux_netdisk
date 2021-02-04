//用相对路径包含头文件
#include "../include/workque.h"

int queInit(pQue_t pQue)
{
    memset(pQue,0,sizeof(que_t));
    //初始化队列中所有成员
    pQue->size = 0;
    pQue->pHead = NULL;
    pQue->pTail= NULL;

    pthread_mutex_init(&pQue->mlock,NULL);
    pthread_cond_init(&pQue->cond,NULL);
 
    return 0;
}

int queInsert(pQue_t pQue,pNode_t pNew)
{
    if(NULL == pQue->pHead )
    {
        //空的队列，插入的就是队列中的第一个元素
        pQue->pHead = pNew;
        pQue->pTail = pNew;
    }
    else{
        pQue->pTail->pNext = pNew;
        pQue->pTail = pNew;
    }
    pQue->size++;
    return 0;
}

int queGet(pQue_t pQue,pNode_t* pGet)
{
    if(0 == pQue->size)
    {
        return -1;
    }

    *pGet = pQue->pHead;
    pQue->pHead = pQue->pHead->pNext;

    if(NULL == pQue->pHead)
    {
        pQue->pTail = NULL;
    }

    pQue->size--;

    return 0;
}
