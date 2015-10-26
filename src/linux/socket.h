#pragma once

#include <functional>

namespace os {

/**
 * tcp stream
 */
class tcp_stream {
public:
	tcp_stream();
	~tcp_stream();

	void open(int sockfd);

	std::string read_str();
	void write_str(const std::string &);

private:
	int sockfd;
	char buffer[1024];

};


/**
 * listens for and accepts new connections
 */
class tcp_acceptor {
public:
	using SocketType = tcp_stream;

	tcp_acceptor(int portno);
	~tcp_acceptor();

	void start(SocketType &stream);

private:
	const int port;

	int sockfd;

};


}
