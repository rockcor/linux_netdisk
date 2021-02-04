//包含的是通用库的头文件

#ifndef __HEAD_H__
#define __HEAD_H__

#define _GUN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>
#include <setjmp.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <sys/shm.h>
#include <sys/epoll.h>

#include <dbg.h>
#define ARGS_CHECK(argc,val) {if(argc!=val) \
    {printf("error args\n"); return -1;}}

#define ERROR_CHECK(ret,retVal,funcName) {if(ret==retVal) {perror(funcName);return -1;}}

#define THREAD_ERRORCHECK(ret,funcName)  do{  if(0 != ret) {  printf("%s : %s \n",funcName,strerror(ret));    }   }while(0)
#define THREAD_ERROR_CHECK(ret,funcName)  do{  if(0 != ret) {  printf("%s : %s \n",funcName,strerror(ret));    }   }while(0)
#define CHILD_THREAD_ERROR_CHECK(ret,funcName)  do{  if(0 != ret) {  printf("%s : %s \n",funcName,strerror(ret)); return (void*)-1;   }   }while(0)

#endif
