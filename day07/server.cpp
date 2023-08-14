#include "src/EventLoop.h"
#include "src/Server.h"

int main(){
    Eventloop *loop = new Eventloop();
    Server *server = new Server(loop);
    loop->loop();
    return 0;
}