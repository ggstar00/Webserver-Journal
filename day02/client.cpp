#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h> //包含<netinet/in.h>
#include <cstring>
#include <unistd.h>

void errif(bool condition,const char *errmsg){
    if(condition){
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv){
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    errif(sockfd == -1,"socket create error");
    struct sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);
    errif(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1,"connect error");
    std::cout << "connected!\n" << std::endl;

    while(true)
    {
        char buf[1024];
        bzero(&buf,sizeof(buf));
        std::cout << "input message:";
        std::cin.getline(buf,1024);
        ssize_t write_bytes = write(sockfd,buf,sizeof(buf));
        if(write_bytes == -1)
        {
            std::cout << "write error" << std::endl;
            break;
        }
        bzero(&buf,sizeof(buf));
        ssize_t read_bytes = read(sockfd,buf,sizeof(buf));
        if(read_bytes == 0)
        {
            std::cout << "server disconnect" << std::endl;
            break;
        }
        else if(read_bytes == -1)
        {
            close(sockfd);
            errif(true,"read error");
        }
        else if(read_bytes > 0)
        {
            std::cout << "message from server:" << buf << std::endl;

        }
    }
}