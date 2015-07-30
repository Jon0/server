#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/asio.hpp>

#include "content.h"
#include "session.h"
#include "user.h"

namespace server {

using boost::asio::ip::tcp;

/**
 * creates sessions and users
 */
class server {
public:
	server(const http::routes &routes, unsigned short port=8080);

	std::vector<std::string> get_sessions() const;

	void run();

	/**
	 * remove a session
	 */
	void stop(session *s);

	/**
	 * create new user
	 */
	user connect(session *s, const std::string &token);

	const http::routes &routes;
	user_set users;

private:
	void accept();
	void log(const std::string &msg);

	boost::asio::io_service io_service;
	tcp::acceptor acceptor;

	// all sessions
	std::unordered_set<std::shared_ptr<session>> sessions;
	std::unordered_map<session *, std::weak_ptr<session>> wp;
};

}
