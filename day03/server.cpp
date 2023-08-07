#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sys/epoll.h>
#include <errno.h>
#include "util.h"
#include <fcntl.h>

#define MAX_EVENTS 1024
#define READ_BUFFER_SIZE 1024

void setnonblocking(int fd){
    fcntl(fd, F_SETFL,fcntl(fd, F_GETFD) | O_NONBLOCK);//fcntl(fd, F_GETFD)获取文件描述符状态标志
}

int main(){
    int sockfd = socket(AF_INET,SOCK_STREAM,0); //创建socket,返回socket文件描述符,第一个参数是协议族，第二个参数是socket类型，第三个参数是协议类型
    errif(sockfd == -1,"socket create error");

    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8888);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    errif(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1 ,"socket bind error");

    errif(listen(sockfd,SOMAXCONN) == -1,"socket listen error");
    std::cout << "server listening on port 8888" << std::endl;
    int epollfd = epoll_create1(0); //创建epoll实例，返回epoll文件描述符
    errif(epollfd == -1 , "epoll create error");

    struct epoll_event events[MAX_EVENTS],ev;
    memset(&events,0,sizeof(events));
    memset(&ev,0,sizeof(ev));
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET; //EPOLLIN:可读事件，EPOLLET:边缘触发
    setnonblocking(sockfd);
    epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev); //注册事件，第一个参数是epoll文件描述符，第二个参数是操作类型，第三个参数是要操作的文件描述符，第四个参数是事件


    while(true){
        int nfds = epoll_wait(epollfd,events,MAX_EVENTS, -1); //等待事件发生，返回发生事件的文件描述符个数
        errif(nfds == -1 ,"epoll wait error");
        for(int i = 0; i < nfds; i++){
            if(events[i].data.fd == sockfd){
                struct sockaddr_in clntaddr;
                memset(&clntaddr,0,sizeof(clntaddr));
                socklen_t clntaddrlen = sizeof(clntaddr);

                int clntfd = accept(sockfd,(struct sockaddr*)&clntaddr,&clntaddrlen); //接收客户端连接，返回新的socket文件描述符
                errif(clntfd == -1,"accept error");

                std::cout << "new client fd" << clntfd << " connection established:" << inet_ntoa(clntaddr.sin_addr) << ":" << ntohs(clntaddr.sin_port) << std::endl;
                memset(&ev,0,sizeof(ev));
                ev.data.fd = clntfd;
                ev.events = EPOLLIN | EPOLLET;
                setnonblocking(clntfd);
                epoll_ctl(epollfd,EPOLL_CTL_ADD,clntfd,&ev);

            }
            else if(events[i].events & EPOLLIN){
                char buf[READ_BUFFER_SIZE];
                while(true){
                    memset(&buf,0,sizeof(buf));
                    ssize_t readlen = read(events[i].data.fd,buf,sizeof(buf));
                    if(readlen > 0)
                    {
                        std::cout << "receive message from client fd" << events[i].data.fd << ":" << buf << std::endl;
                        //memset(&buf,0,sizeof(buf));
                        //std::cin.getline(buf,sizeof(buf));
                        write(events[i].data.fd,buf,sizeof(buf));
                    }
                    else if(readlen == -1){
                        if(errno == EAGAIN || errno == EWOULDBLOCK){   //EAGAIN:资源暂时不可用，EWOULDBLOCK:操作被阻塞
                            break;
                        }
                        else if(errno == EINTR){
                            continue;
                        }
                        else{
                            std::cout << "read error" << std::endl;
                            close(events[i].data.fd);
                            break;
                        }
                    }
                    else if(readlen == 0)
                    {
                        std::cout << "client fd" << events[i].data.fd << "close connection" << std::endl;
                        close(events[i].data.fd);
                        break;
                    }
                }
            }
            else{
                std::cout << "other event" << std::endl; //其他事件，如EPOLLOUT:可写事件，EPOLLERR:错误事件，EPOLLHUP:挂起事件，EPOLLRDHUP:对端关闭连接事件，EPOLLONESHOT:一次性事件，只触发一次，后续需要重新添加
            }
        }
    }
    close(sockfd);
    close(epollfd);
    return 0;
}