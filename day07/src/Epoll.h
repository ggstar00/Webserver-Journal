#pragma once
#include <sys/epoll.h>
#include <vector>


class Channel;
class Epoll
{
private:
    int epfd;
    struct epoll_event* events;

public:
    Epoll();
    ~Epoll();

    void addFd(int fd,uint32_t events);
    void updateChannel(Channel*);
    void delFd(int fd);

    std::vector<Channel*> poll(int timeout = -1);
};