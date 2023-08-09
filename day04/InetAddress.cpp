#include "InetAddress.h"
#include <string.h>

InetAddress::InetAddress():addr_len(sizeof(addr)){
    bzero(&addr,addr_len);
}
InetAddress::InetAddress(const char* ip,int port):addr_len(sizeof(addr)){
    bzero(&addr,addr_len);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    addr_len = sizeof(addr);
}