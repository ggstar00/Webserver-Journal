#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <vector>

Eventloop::Eventloop() : ep(nullptr),quit(false){
    ep = new Epoll();
}

Eventloop::~Eventloop(){
    delete ep;
}

void Eventloop::loop(){
    while(!quit){
        std::vector<Channel*> chs;
        chs = ep->poll();
        for(auto it = chs.begin(); it != chs.end(); ++it){
            (*it)->handleEvent();
        }
    }
}

void Eventloop::updateChannel(Channel *ch){
    ep->updateChannel(ch);
}