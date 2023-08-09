#pragma once
#include <sys/epoll.h>
#include <vector>

class Epoll
{
private:
    int epfd;
    struct epoll_event* events;

public:
    Epoll();
    ~Epoll();

    void addFd(int fd,uint32_t events);
    void delFd(int fd);

    std::vector<epoll_event> poll(int timeout = -1);
};