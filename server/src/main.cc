#include "../include/head.h"
#include "../include/threadpool.h"

int fds[2];
void exitFun(int signum)
{
    char exitFlag=1;
    write(fds[1],&exitFlag,sizeof(exitFlag));
    printf("signal %d is comming\n",signum);
}
// ip，port,threadnum
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,4);

    pipe(fds);
    signal(SIGUSR1,exitFun);

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
    epollAdd(epfd,fds[0]);
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
            else
            {
                threadPool.startFlag=0;
                for(int j=0;j<threadPool.threadnum;j++)
                {
                    pthread_cond_signal(&threadPool.que.cond);
                    pthread_join(threadPool.pThid[j],NULL);
                }
                printf("main thread exit\n");
                close(listenFd);
                exit(0);
            }
        }
    }
    return 0;
}
