#include "socket.h"
#include "util.h"
#include "InetAddress.h"
#include "unistd.h"
#include <fcntl.h> //包含fcntl函数,open函数,O_NONBLOCK常量
#include <sys/socket.h> //包含socket函数,bind函数,listen函数,accept函数,connect函数,shutdown函数,send函数,recv函数,sendto函数,recvfrom函数,sendmsg函数,recvmsg函数
#include <iostream>

Socket::Socket():fd(-1){
    fd = socket(AF_INET,SOCK_STREAM,0);
    errif(fd == -1,"socket create error");
}

Socket::Socket(int _fd):fd(_fd){
    errif(fd == -1,"socket create error");
}

Socket::~Socket(){
    if(fd != -1){
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress *addr){
    errif(::bind(fd,(struct sockaddr*)&addr->addr,addr->addr_len) == -1,"socket bind error"); //::bind函数，::表示全局命名空间
    std::cout << "bind success" << std::endl;
}

void Socket::listen(){
    errif(::listen(fd,SOMAXCONN) == -1,"socket listen error");
    std::cout << "listen success" << std::endl;
}

void Socket::setnonblocking(){
    fcntl(fd,F_SETFL,fcntl(fd,F_GETFD) | O_NONBLOCK);
}

int Socket::accept(InetAddress *addr){
    int clnt_sockfd = ::accept(fd,(sockaddr*)&addr->addr,&addr->addr_len);
    errif(clnt_sockfd == -1,"socket accept error");
    return clnt_sockfd;
}

int Socket::getfd(){
    return fd;
}

void Socket::connect(InetAddress *addr){
    errif(::connect(fd,(sockaddr*)&addr->addr,addr->addr_len) == -1,"socket connect error");
    std::cout << "connect success" << std::endl;
}
