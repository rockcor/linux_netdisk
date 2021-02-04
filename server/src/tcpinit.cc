/*************************************************************************
	> File Name: server.c
	> Author: Name
	> Mail: Name@163.com 
	> Created Time: 2021-01-29 17:25:48
 ************************************************************************/

#include "../include/threadpool.h"

int tcpInit(int* socketFd,char* ip,char* port)
{

    //先创建监听套接字,用于接收服务端的连接请求
    int listenFd= socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(listenFd,-1,"socket");

    //设置地址可重用，允许绑定TIME_WAIT状态的地址
    //
    int reuse = 1;
    int ret = 0;
    ret = setsockopt(listenFd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    ERROR_CHECK(ret,-1,"setsockopt");

    //定义结构体，记录ip和端口
    struct sockaddr_in serAddr;
    memset(&serAddr,0,sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    //把字符串的点分十进制ip转换成网络字节序的二进制ip
    serAddr.sin_addr.s_addr = inet_addr(ip);
    serAddr.sin_port = htons(atoi(port));

    //绑定ip和端口号
    ret = bind(listenFd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    ERROR_CHECK(ret,-1,"bind");

    //建立监听，等待客户端连接
    //10表示全连接队列的大小
    ret = listen(listenFd,10); 
    ERROR_CHECK(ret,-1,"listen");
    
    *socketFd = listenFd;

    return 0;
}
