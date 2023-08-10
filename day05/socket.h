#pragma once // pragma once 只能保证一个文件只被编译一次，但是不能保证多个文件只被编译一次

class InetAddress;
class Socket
{
private:
    int fd;
public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress*);
    void listen();
    void setnonblocking();
    void connect(InetAddress*);

    int accept(InetAddress*);
    int getfd();
};