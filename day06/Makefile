server:
	g++-11 src/util.cpp client.cpp src/socket.cpp src/InetAddress.cpp -o client && g++-11 src/util.cpp server.cpp src/Epoll.cpp src/InetAddress.cpp src/socket.cpp src/Channel.cpp src/EventLoop.cpp src/Server.cpp -o server
clean:
	rm server && rm client