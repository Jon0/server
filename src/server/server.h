#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include "content.h"
#include "session.h"
#include "user.h"

namespace server {

/**
 * creates sessions and users
 */
template<typename AcceptorType>
class tcp_server {
public:
	using SocketType = typename AcceptorType::SocketType;
	using SessionType = session_handler<tcp_server<AcceptorType>>;
	using ElementType = std::shared_ptr<SessionType>;

	tcp_server(const http::routes &routes, unsigned short port=8080)
		:
		routes(routes) {
	}

	void run() {
		AcceptorType acceptor(8080);

		while (true) {
			SessionType new_session(this);
			acceptor.start(new_session.socket);
			new_session.start();
		}
	}

	/**
	 * remove a session
	 */
	void stop(ElementType s) {
		sessions.erase(s);

		// one session reserved for new connections
		log(std::to_string(sessions.size() - 1) + " active sessions");
	}

	/**
	 * create new user
	 */
	user connect(ElementType s) {
		if (!s) {
			log("connecting invalid session");
		}

		// find or create the user
		user u = users.new_user();
		
		//u.link_session(wp[s].lock());
		return u;
	}

	const http::routes &routes;
	user_set users;

private:
	void log(const std::string &msg) {
		std::cout << "[server] " << msg << "\n";
	}

	// all sessions
	std::unordered_set<std::shared_ptr<SessionType>> sessions;

};


}
