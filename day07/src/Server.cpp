#include "Server.h"
#include "socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Acceptor.h"
#include <functional>
#include <string.h>
#include <unistd.h>
#include <iostream>

#define READ_BUFFER 1024

Server::Server(Eventloop *_loop): loop(_loop){
    // Socket *serv_sock = new Socket();
    // InetAddress *serv_addr = new InetAddress("127.0.0.1",8888);
    // serv_sock->bind(serv_addr);
    // serv_sock->listen();
    // serv_sock->setnonblocking();
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection,this,std::placeholders::_1);//std::placeholders::_1表示占位符，表示回调函数的参数
    acceptor->setNewConnectionCallback(cb);//设置回调函数,当有新连接时调用,并将serv_sock传入,以便在newConnection中accept
}

Server::~Server(){
    delete acceptor;
}

void Server::handleReadEvent(int sockfd){
    char buf[READ_BUFFER];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        memset(&buf, 0, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            std::cout << "message from client fd " << sockfd << ":" << buf << std::endl;
            write(sockfd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            std::cout << "continue reading" << std::endl;
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            std::cout << "finish reading once, errno: " << errno << std::endl;
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            std::cout << "EOF, client fd " << sockfd << " disconnected" << std::endl;
            close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}

void Server::newConnection(Socket *serv_sock){
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));
    std::cout << "new client fd " << clnt_sock->getFD() << " connection established: " << inet_ntoa(clnt_addr->addr.sin_addr) << ":" << ntohs(clnt_addr->addr.sin_port) << std::endl;
    clnt_sock->setnonblocking();
    Channel *clntCh = new Channel(loop,clnt_sock->getFD());
    std::function<void()> cb = std::bind(&Server::handleReadEvent,this,clnt_sock->getFD());
    clntCh->setCallback(cb);
    clntCh->enableReading();
}