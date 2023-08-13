#pragma once
class Epoll;
class Channel;
class Eventloop
{
private:
    Epoll *ep;
    bool quit;
public:
    Eventloop();
    ~Eventloop();

    void loop();
    void updateChannel(Channel*);
};