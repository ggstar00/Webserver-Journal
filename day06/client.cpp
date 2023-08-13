#include <iostream>
#include "src/socket.h"
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "src/util.h"
#include "src/InetAddress.h"

#define READ_BUFFER_SIZE 1024

int main(){
    Socket *sockfd = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1",8888);

    sockfd->connect(serv_addr);

    while(true){
        char buf[READ_BUFFER_SIZE];
        memset(&buf,0,sizeof(buf));
        std::cin.getline(buf,sizeof(buf));
        ssize_t writebuf = write(sockfd->getFD(),buf,sizeof(buf));
        if(writebuf == -1){
            std::cout << "socket connected,but not write " << std::endl;
            break;
        }
        memset(&buf,0,sizeof(buf));
        ssize_t readbuf = read(sockfd->getFD(),buf,sizeof(buf));
        if(readbuf > 0){
            std::cout << "message from server:" << buf << std::endl;

        }else if(readbuf == 0){
            std::cout << "server disconnectd" << std::endl;
        }else if(readbuf == -1){
            delete sockfd;
            errif(true,"socket read error");
        }

    }
    delete sockfd;
    return 0;


}