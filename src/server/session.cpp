#include <iostream>

#include "server.h"
#include "session.h"

namespace server {

session::session(boost::asio::io_service &io_service, server *s)
	:
	socket(io_service),
	srv(s),
	r_name("closed"),
	parse_token(true) {
}

session::~session() {
	log("connection closed");
}

std::string session::remote_name() const {
	return r_name;
}

user session::get_user() const {
	return id;
}

void session::start() {
	set_remote_name();
	log("connection accepted");
	read();
}

void session::write(const std::string &str) {

	// TODO large strings are not completely written
	boost::asio::async_write(socket, boost::asio::buffer(str),
		[this](boost::system::error_code ec, std::size_t /*length*/) {
			if (!ec) {
				log("write complete");
			}
	});
}


void session::read() {

	socket.async_read_some(boost::asio::buffer(request_buffer, buffer_size),
		[this](boost::system::error_code ec, std::size_t length) {
			if (!ec) {
				respond(std::string(request_buffer, length));
				read();
			}
			else if (ec != boost::asio::error::operation_aborted) {
				log("connection shutdown");

				// TODO clear in destructor
				active.clear();
				srv->stop(this);
			}
	});

}

void session::respond(const std::string &req_str) {

	// parse the request
	http::request request = http::parse_request(req_str);
	log("request for "+request.location);

	// cookie things
	bool send_token = false;
	if (parse_token) {

		// for the first read operation
		send_token = connect_user(request);
		parse_token = false;
	}
	request.from = id;
	request.remote = r_name;

	// debug requests
	id.log(request.info());

	// add to active list
	active.emplace_back(srv->routes.get_response(request));
	if (send_token) {
		active.back().include_token(request.from.token());
	}
	active.back().send(request, [this](const std::string &s){
		write(s);
	});

}

void session::log(const std::string &msg) {
	std::cout << "[" << r_name << "] " << msg << "\n";
}

void session::set_remote_name() {
	const tcp::endpoint &ep = socket.remote_endpoint();
	r_name = ep.address().to_string() + ":" + std::to_string(ep.port());
}

bool session::connect_user(const http::request &request) {
	std::string token = "";
	if (request.data.count("sessionToken") > 0) {
		token = request.data.at("sessionToken");
	}

	// store user as id
	id = srv->connect(this, token);
	if (!id.valid()) {
		log("connect returned invalid user");
	}
	return token.empty();
}

} // namespace server
