#pragma once

#include <boost/asio.hpp>

#include "http.h"
#include "routes.h"

namespace server {

using boost::asio::ip::tcp;

constexpr int buffer_size = 1024;

class server;

class session {
public:
	session(boost::asio::io_service &io_service, server *s);
	~session();

	std::string remote_name() const;
	user get_user() const;

	void start();

	void write(const std::string &str);

	tcp::socket socket;

private:
	void read();
	void respond(const std::string &req_str);
	void log(const std::string &msg);

	void set_remote_name();
	bool connect_user(const http::request &request);


	server *srv;
	user id;
	std::string r_name;
	std::vector<http::response> active;
	char request_buffer [buffer_size];
	bool parse_token;
};

}
