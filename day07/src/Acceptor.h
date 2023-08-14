#pragma once
#include <functional>

class Eventloop;
class Socket;
class InetAddress;
class Channel;
class Acceptor
{
private:
    Eventloop *loop;
    Socket *sock;
    InetAddress *addr;
    Channel *acceptChannel;
public:
    Acceptor(Eventloop *_loop);
    ~Acceptor();
    void acceptConnection();
    std::function<void(Socket*)> newConnectionCallback;
    void setNewConnectionCallback(std::function<void(Socket*)>);
};