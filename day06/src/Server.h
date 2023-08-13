#pragma once 

class Eventloop;
class Socket;
class Server{
private:
    Eventloop *loop;

public:
    Server(Eventloop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock); 
};