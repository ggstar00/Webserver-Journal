#pragma once
#include <sys/epoll.h>
#include <functional>

class Eventloop; // 前向声明,解决循环引用,避免头文件相互包含,导致编译错误,但是前向声明不能使用类的成员,只能使用指针,因为编译器不知道类的大小,只知道指针的大小,所以可以使用指针
class Channel
{
private:
    Eventloop *loop;
    int fd;
    uint32_t events; //监听事件
    uint32_t revents; //返回事件
    bool inEpoll; //判断是否在epoll中
    std::function<void()> callback;
public:
    Channel(Eventloop *,int);
    ~Channel();

    void enableReading();

    int getfd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getinEpoll();
    void setinEpoll();

    void setRevents(uint32_t);
    void handleEvent();
    void setCallback(std::function<void()> _cb);
};