#pragma once

class InetAddress;
class Socket
{
private:
    int fd;
public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress* addr);
    void listen();
    int accept(InetAddress* addr);

    void setnonblocking();
    int getFD();
    void connect(InetAddress*);
};
