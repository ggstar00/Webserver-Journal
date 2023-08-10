#include <iostream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include "util.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "socket.h"
#include "Channel.h"

#define MAX_EVENTS 1024
#define READ_BUFFER_SIZE 1024

void handleReadEvent(int);

int main(){
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1",8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    Epoll *ep = new Epoll();
    serv_sock->setnonblocking();
    //ep->addFd(serv_sock->getfd(),EPOLLIN | EPOLLET); //出现bug，未修改inEpoll值导致后面Epoll里调用enableReading()时出错，已修复
    Channel *serv_ch = new Channel(ep,serv_sock->getfd()); 
    serv_ch->enableReading();
    while(true){
        std::vector<Channel*> activateChannels = ep->poll();
        std::cout << "activateChannels size:" << activateChannels.size() << std::endl;
        int nfds = activateChannels.size();
        for(int i = 0;i < nfds;++i){
            int chfd = activateChannels[i]->getfd();
            if(chfd == serv_sock->getfd()){
                InetAddress *clnt_addr = new InetAddress();
                Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr)); 
                std::cout << "new client fd" << clnt_sock->getfd() << " connection established:" << inet_ntoa(clnt_addr->addr.sin_addr) << ":" << ntohs(clnt_addr->addr.sin_port) << std::endl;
                clnt_sock->setnonblocking();
                Channel *clnt_ch = new Channel(ep,clnt_sock->getfd());
                clnt_ch->enableReading();
            }else if(activateChannels[i]->getRevents() & EPOLLIN){
                handleReadEvent(activateChannels[i]->getfd());
            }else{
                std::cout << "something else happened" << std::endl;
            }
        }
    }
    delete serv_sock;
    delete serv_addr;
    return 0;
}

void handleReadEvent(int sockfd){
    char buf[READ_BUFFER_SIZE];
    while(true){
        memset(&buf,0,sizeof(buf));
        ssize_t readbuf = read(sockfd,buf,sizeof(buf)); 
        if(readbuf > 0){
            std::cout << "message from client fd " << sockfd << ":" << buf << std::endl;
            write(sockfd,buf,sizeof(buf));
        }else if(readbuf == -1 && errno == EINTR){
            std::cout << "continue reading" << std::endl;
            continue;
        }else if(readbuf == -1 && (errno == EAGAIN) || (errno == EWOULDBLOCK)){
            std::cout << "finish reading once , errno:" << errno << std::endl;
            break;
        }else if(readbuf == 0){
            std::cout << "EOF,client fd " << sockfd << "disconnected" << std::endl;
            close(sockfd);
            break;
        }
    }
}