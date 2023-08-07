#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "util.h"

#define BUFFER_SIZE 1024

int main(){
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    errif(sockfd == -1,"socket create error");

    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8888);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");


    errif(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1, "connect error");

    while(true){
        char buf[BUFFER_SIZE];
        memset(&buf,0,sizeof(buf));
        std::cout << "请输入要发送的消息:";
        std::cin.getline(buf,sizeof(buf));

        ssize_t write_size = write(sockfd,buf,sizeof(buf));
        errif(write_size == -1,"write error");

        if(write_size == -1)
        {
            std::cout << "client disconnected" << std::endl;
            break;
        }
        
        memset(&buf,0,sizeof(buf));
        ssize_t read_size = read(sockfd,buf,sizeof(buf));
        if(read_size == -1)
        {
            std::cout << "read error" << std::endl;
            break;
        }
        else if(read_size == 0)
        {
            std::cout << "server disconnected" << std::endl;
            break;
        }
        else if(read_size > 0)
        {
            std::cout << "server:" << buf << std::endl;
        }
    }
    close(sockfd);
    return 0; 
}