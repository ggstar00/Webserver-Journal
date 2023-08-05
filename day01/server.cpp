#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h> //包含<netinet/in.h>
#include <cstring>


int main(int argc, char **argv)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // 创建socket文件描述符，第一个参数是协议族，这里是指ipv4，第二个参数是socket类型，流格式，多用于TCP，第三个参数是协议，一般为0，表示根据前面两个参数自动选择

    struct sockaddr_in serveraddr; // 定义一个ipv4的地址结构体
    bzero(&serveraddr,sizeof(serveraddr)); // 将结构体清零

    serveraddr.sin_family = AF_INET; // 指定协议族
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");// 指定ip地址
    serveraddr.sin_port = htons(8888);// 指定端口号
    
    bind(sockfd, (struct sockaddr*)&serveraddr,sizeof(serveraddr)); // 将socket文件描述符和地址结构体绑定
    listen(sockfd, SOMAXCONN); // 开始监听，第二个参数是最大监听数
    std::cout << "listening...\n" << std::endl;
    struct sockaddr_in client_addr; // 定义一个ipv4的地址结构体
    socklen_t clnt_addr_len = sizeof(client_addr); // 定义一个长度变量，用于接收客户端地址结构体的长度
    bzero(&client_addr,sizeof(client_addr)); //将结构体清零，遵循初始化原则
    int clnt_sockfd = accept(sockfd,(sockaddr*)&client_addr,&clnt_addr_len); // 接收客户端的连接，第一个参数是socket文件描述符，第二个参数是客户端地址结构体，第三个参数是客户端地址结构体的长度
    std::cout << "new client fd %d! IP: %s, Port: %d\n" << clnt_sockfd << inet_ntoa(client_addr.sin_addr) << ntohs(client_addr.sin_port) << std::endl; // 打印客户端的ip和端口号
}








