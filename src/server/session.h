#pragma once

#include <iostream>

#include "http.h"
#include "routes.h"

namespace server {

class session {
public:
	std::string remote_name() {
		return "remote";
	}
};

template<typename ServerType>
class session_handler {
public:
	using SocketType = typename ServerType::SocketType;

	session_handler(ServerType *s)
		:
		srv(s),
		r_name("closed"),
		parse_token(true) {
	}

	void start() {
		set_remote_name();
		log("connection accepted");
		read();
	}

	void write(const std::string &str) {
		socket.write_str(str);
	}

	SocketType socket;

private:
	void read() {
		respond(socket.read_str());
	}

	void respond(const std::string &req_str) {

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

	void log(const std::string &msg) {
		std::cout << "[" << r_name << "] " << msg << "\n";
	}

	void set_remote_name() {
		r_name = "open";
	}

	bool connect_user(const http::request &request) {
		std::string token = "";
		if (request.data.count("sessionToken") > 0) {
			token = request.data.at("sessionToken");
		}

		// store user as id
		//id = srv->connect(this, token);
		//if (!id.valid()) {
		//	log("connect returned invalid user");
		//}
		return token.empty();
	}

	ServerType *srv;
	user id;
	std::string r_name;
	std::vector<http::response> active;
	bool parse_token;
};

}
