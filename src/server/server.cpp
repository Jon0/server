#include <iostream>

#include "server.h"

namespace server {

server::server(const http::routes &routes, unsigned short port)
	:
	routes(routes),
	acceptor(io_service, tcp::endpoint(tcp::v4(), port)) {
	accept();
}

std::vector<std::string> server::get_sessions() const {
	std::vector<std::string> result;
	for (auto &s : sessions) {
		const std::string &name = s->get_user().name();
		if (!name.empty()) {
			result.push_back(name);
		}
	}
	return result;
}

void server::run() {
	log("accepting connections");
	io_service.run();
}

void server::stop(session *s) {
	if (wp.count(s) > 0) {
		auto shared_session = wp[s].lock();

		// unlink from the sessions user
		user u = shared_session->get_user();
		u.unlink_session(shared_session);

		// erase from servers active sessions
		sessions.erase(shared_session);
		wp.erase(s);
	}

	// one session reserved for new connections
	log(std::to_string(sessions.size() - 1) + " active sessions");
}

user server::connect(session *s, const std::string &token) {
	if (!s || wp.count(s) == 0) {
		log("connecting invalid session");
	}

	// find or create the user
	user u;
	if (token.empty()) {
		u = users.new_user();
	}
	else {
		u = users.by_token(token);
		if (!u.valid()) {
			u = users.new_user(token);
		}
	}

	u.link_session(wp[s].lock());
	return u;
}

void server::accept() {
	auto s = *sessions.emplace(std::make_shared<session>(io_service, this)).first;
	wp.emplace(std::make_pair(s.get(), std::weak_ptr<session>(s)));

	// queues a job for the io_service
	acceptor.async_accept(s->socket, [this, s](boost::system::error_code ec) {
		if (!ec) {
			s->start();
			this->accept();
		}
	});
}

void server::log(const std::string &msg) {
	std::cout << "[server] " << msg << "\n";
}



}
