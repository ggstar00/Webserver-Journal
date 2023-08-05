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


int main(int argc,char **argv){
    int sockfd = socket(AF_INET,SOCK_STREAM,0);// 创建socket文件描述符，第一个参数是协议族，这里是指ipv4，第二个参数是socket类型，流格式，多用于TCP，第三个参数是协议，一般为0，表示根据前面两个参数自动选择
    errif(sockfd == -1, "socket create error");
    struct sockaddr_in serv_addr;//定义一个ipv4的地址结构体
    bzero(&serv_addr,sizeof(serv_addr));//将结构体清零，遵循初始化原则

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(8888);

    //bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    errif(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1,"bind");
    errif(listen(sockfd,SOMAXCONN) == -1,"listen error");
    

    std::cout << "listening...\n" << std::endl;

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr,sizeof(sockaddr));
    int clntfd = accept(sockfd,(sockaddr*)&clnt_addr,&clnt_addr_len);
    errif(clntfd == -1,"accept error");
    std::cout << "new client fd "<< clntfd << "! IP " << inet_ntoa(clnt_addr.sin_addr) <<  ", Port "   << ntohs(clnt_addr.sin_port) << std::endl;
    
    while(true){
        char buf[1024]; // 定义一个缓冲区
        bzero(&buf,sizeof(buf));//将缓冲区清零
        ssize_t read_bytes = read(clntfd,buf,sizeof(buf));// 读取客户端发送的数据
        if(read_bytes == 0){
            std::cout << "client fd " << clntfd << " disconnect"  << std::endl;
            close(clntfd);
            break;
        }
        else if(read_bytes == -1){
            close(clntfd);
            errif(true,"read error");
        }
        else
        {
            std::cout << "client fd "<< clntfd << "  send:" << buf << std::endl;
            bzero(&buf,sizeof(buf));
            std::cout << "input message:";
            std::cin.getline(buf,1024);
            write(clntfd,buf,sizeof(buf));// 将数据回写给客户端
        }

    }
}