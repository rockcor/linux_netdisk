#include "../include/threadpool.h"

int epollAdd(int epfd,int fd)
{
    struct epoll_event event;
    memset(&event,0,sizeof(event));

    event.data.fd = fd;
    event.events = EPOLLIN;

    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    ERROR_CHECK(ret,-1,"epoll_add");
 
    return 0;
}
