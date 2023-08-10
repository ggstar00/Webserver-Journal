#include "Channel.h"
#include "Epoll.h"

Channel::Channel(Epoll *_ep,int _fd) : ep(_ep),fd(_fd), events(0),revents(0),inEpoll(false){

}

Channel::~Channel()
{

}

void Channel::enableReading()
{
    events = EPOLLIN | EPOLLET;//设置可读和边缘触发
    ep->updateChannel(this);
}

int Channel::getfd()
{
    return fd;
}

uint32_t Channel::getEvents()
{
    return events;
}

uint32_t Channel::getRevents()
{
    return revents;
}

bool Channel::getinEpoll()
{
    return inEpoll;
}

void Channel::setinEpoll()
{
    inEpoll = true;
}

void Channel::setRevents(uint32_t _ev)
{
    revents = _ev;
}