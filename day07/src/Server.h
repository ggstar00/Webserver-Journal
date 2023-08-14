#pragma once 

class Eventloop;
class Socket;
class Acceptor;
class Server{
private:
    Eventloop *loop;
    Acceptor *acceptor;
public:
    Server(Eventloop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock); 
};