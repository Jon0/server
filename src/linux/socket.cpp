#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "socket.h"

namespace os {

void error(const char *msg) {
	perror(msg);
	exit(1);
}

tcp_acceptor::tcp_acceptor(int portno)
	:
	port(portno) {

	// open socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket");
	}

	// bind and listen on socket
	sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);	
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR on binding");
	}
	listen(sockfd, 5);
}

tcp_acceptor::~tcp_acceptor() {
	close(sockfd);
}

void tcp_acceptor::start(SocketType &stream) {
	sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);
	int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if (newsockfd < 0) {
		error("ERROR on accept");
	}

	stream.open(newsockfd);
}

tcp_stream::tcp_stream()
	:
	sockfd(0) {
}

tcp_stream::~tcp_stream() {
	close(sockfd);
}

void tcp_stream::open(int newsockfd) {
	sockfd = newsockfd;
}

std::string tcp_stream::read_str() {
	int n = read(sockfd, buffer, 1024);
	if (n < 0) error("ERROR reading from socket");

	std::string result = std::string(buffer, n);
	return result;
}

void tcp_stream::write_str(const std::string &str) {
	int n = write(sockfd, str.c_str(), str.length());
	if (n < 0) error("ERROR writing to socket");
}

}
