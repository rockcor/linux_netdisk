/*************************************************************************
	> File Name: main.c
	> Author: Name
	> Mail: Name@163.com 
	> Created Time: 2021-02-04 16:06:27
 ************************************************************************/

#include "../include/head.h"
#include "../include/threadpool.h"

// ip，port,threadnum
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,4);

    int threadnum = atoi(argv[3]);
    threadpool_t threadPool;
    memset(&threadPool,0,sizeof(threadPool));

    //1.初始化线程池
    threadPoolInit(&threadPool,threadnum);

    //2.启动线程池
    threadPoolStart(&threadPool);

    //3.建立TCP监听
    int listenFd = 0;
    tcpInit(&listenFd,argv[1],argv[2]);

    //4.监听套接字
    int epfd=epoll_create(1);
    ERROR_CHECK(epfd,-1,"epoll_create");
    epollAdd(epfd,listenFd);
    int readyNum=0;
    struct epoll_event evs[2];
    memset(evs,0,sizeof(evs));
    int clientFd=0;
    while(1)
    {
        readyNum=epoll_wait(epfd,evs,2,-1);
        for(int i=0;i<readyNum;i++)
        {
            if(evs[i].data.fd==listenFd)
            {
                clientFd=accept(listenFd,NULL,NULL);
                ERROR_CHECK(clientFd,-1,"accept");
                pNode_t pNew=(pNode_t)calloc(1,sizeof(node_t));
                pNew->clientFd=clientFd;
                pthread_mutex_lock(&threadPool.que.mlock);
                queInsert(&threadPool.que,pNew);
                pthread_mutex_unlock(&threadPool.que.mlock);
                pthread_cond_signal(&threadPool.que.cond);
            }
        }
    }
    close(listenFd);
    return 0;
}
