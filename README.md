web服务器学习笔记：
day01 服务端和客户端单对单的简单收发;  
day02 在day01基础上添加错误处理，实际上更复杂;  
day03 服务端和客户端单对多的简单收发，使用epoll实现并发;  
day04 将day03功能抽象成为类，面向对象;  
day05 抽象epoll事件，使用Channel类管理事件，可以连接不同类型的事件;  
day06 Reactor模式，将Channel类和EventLoop类分离，EventLoop类负责事件循环，Channel类负责事件处理;  
day07 分离Acceptor类，负责监听新连接，将新连接交给EventLoop类处理;  