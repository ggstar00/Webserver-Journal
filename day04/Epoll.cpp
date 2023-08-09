#include "Epoll.h"
#include "util.h"
#include <unistd.h> //包含close函数,read函数,write函数,usleep函数,STDIN_FILENO常量,STDOUT_FILENO常量,STDERR_FILENO常量
#include <cstring>  //包含memset函数

#define MAX_EVENTS 1024

Epoll::Epoll():epfd(-1),events(nullptr){  //初始化，创建epoll实例，创建epoll_event数组并且初始化为0
    epfd = epoll_create1(0);
    errif(epfd == -1,"epoll_create1");
    events = new epoll_event[MAX_EVENTS];
    memset(events,0,sizeof(*events));
}

Epoll::~Epoll(){
    if(epfd != -1){
        close(epfd);
        epfd = -1;
    }
}

void Epoll::addFd(int fd,uint32_t events){
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.fd = fd;
    ev.events = events;
    errif(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev) == -1,"epoll add event error");//epoll_ctl函数用于控制epoll实例，EPOLL_CTL_ADD表示添加事件，fd表示要添加的文件描述符，ev表示要添加的事件，第一个参数是epoll实例，第二个参数是控制命令，第三个参数是要控制的文件描述符，第四个参数是要控制的事件
}

std::vector<epoll_event> Epoll::poll(int timeout){  
    std::vector<epoll_event> activeEvents;
    int nfds = epoll_wait(epfd,events,MAX_EVENTS,timeout);
    errif(nfds == -1,"epoll_wait");
    for(int i = 0;i < nfds;++i){
        activeEvents.push_back(events[i]); //将活跃的事件添加到activeEvents中
    }
    return activeEvents;
}