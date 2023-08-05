#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h> //包含<netinet/in.h>
#include <cstring>


int main(int argc,char **argv)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);// 创建socket文件描述符，第一个参数是协议族，这里是指ipv4，第二个参数是socket类型，流格式，多用于TCP，第三个参数是协议，一般为0，表示根据前面两个参数自动选择
    struct sockaddr_in serv_addr;// 定义一个ipv4的地址结构体
    bzero(&serv_addr,sizeof(serv_addr));//将结构体清零，遵循初始化原则
    serv_addr.sin_family = AF_INET;//指定协议族
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//指定ip地址
    serv_addr.sin_port = htons(8888);//指定端口号
    connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));//连接服务器，第一个参数是socket文件描述符，第二个参数是服务器地址结构体，第三个参数是服务器地址结构体的长度
    std::cout << "connected!\n" << std::endl;
}
