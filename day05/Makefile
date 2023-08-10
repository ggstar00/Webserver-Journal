
server:
	g++ util.cpp client.cpp socket.cpp InetAddress.cpp -o client && g++ util.cpp server.cpp Epoll.cpp InetAddress.cpp socket.cpp Channel.cpp -o server
clean:
	rm server && rm client