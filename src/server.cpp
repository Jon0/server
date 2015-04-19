#include "server.h"

namespace io {

void server::end_session(session *to_remove) {
	session_lock.lock();
	sessions.erase(
		std::remove_if(
			std::begin(sessions),
			std::end(sessions),
			[to_remove](std::shared_ptr<session> e) {
				return e.get() == to_remove;
			}), 
		std::end(sessions));
	session_lock.unlock();
}

void server::do_accept() {
	auto s = std::make_shared<session>(*this, update_function);

	acceptor_.async_accept(s->socket(),
		[this, s](boost::system::error_code ec) {

			// start session and add to list
			if (!ec && s->start()) {
				session_lock.lock();
				sessions.push_back(s);
				session_lock.unlock();
			}
			do_accept();
		});
}

std::string server::status() {
	std::string result;

	result += std::to_string(sessions.size()) + " sessions active\n\n";
	for (auto &s : sessions) {
		result += s->description() + "\n";
	}
	return result;
}

}