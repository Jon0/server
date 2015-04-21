#ifndef SESSION_H
#define SESSION_H

#include <cstdlib>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <utility>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "system/log.h"
#include "parser.h"

namespace io {

using boost::asio::ip::tcp;

using ssl_socket = boost::asio::ssl::stream<tcp::socket>;

const std::string newline = "\r\n";

class server;

enum class session_state {
	initial,
	idle,
	active,
	stoping,
	stopped
};

/**
 * tcp connection with a single client
 */
class session : public std::enable_shared_from_this<session> {
public:

	/**
	 * create session with socket and root directory of served files
	 * and a callback function for any updates from the client
	 */
	session(server &s,
			std::function<void(http::str_map)> &func);

	~session();

	tcp::socket &socket() {
		return socket_;
	}

	/**
	 * start responding to http requests
	 *
	 * return true if startup was sucsessful
	 */
	bool start() {
		if (this->state != session_state::initial) {
			std::cout << "error: cannot start again" << std::endl;
			return false;
		}

		start_time = std::chrono::high_resolution_clock::now();
		sys::log() << "start session with " << socket().remote_endpoint().address().to_string() 
					<< " (id: " << id << ")\n";

		this->state = session_state::idle;
		start_write_thread();
		do_read();
		return true;
	}

	/**
	 * write to the out stream when it is active
	 */
	void send(std::string in) {
		if (this->state == session_state::active) {
			std::string content = "data: " + in + newline;
			content += newline;
			msg(content);
		}
	}

	void end();

	std::string description();

private:

	/**
	 * read incoming http requests
	 */
	void do_read();

	/**
	 * write a string with an http header to socket
	 */
	void write_string(const std::string &str, const std::string &type);

	/**
	 * write a file with an http header to socket, using the requested filename
	 */
	void write_page(const std::string &filename);

	/**
	 * write stream http header to socket
	 */
	void write_stream();

	void start_write_thread();

	/**
	 * non-blocking function to queue writing message to client
	 */
	void msg(const std::string &content) {
		int i = 0;

		// break into blocks and add to queue
		this->queue_lock.lock();
		while (i < content.size()) {
			this->write_queue.push(content.substr(i, max_blocksize));
			i += max_blocksize;
		}
		this->queue_lock.unlock();
	}

	/**
	 * map requested locations to actual files
	 */
	std::string get_location(std::string in_location) {
		if (in_location == "/") {
			return "/index.html";
		}
		else {
			return in_location;
		}
	}

	/**
	 * unique id for the session
	 */
	static int next_id;
	int id;

	/**
	 * server which created this
	 */
	server &create_server;

	/**
	 * current stat of connection
	 */
	session_state state;

	/**
	 * only allow one write operation at a time
	 */
	std::thread write_thread;
	std::queue<std::string> write_queue;
	std::mutex queue_lock;

	tcp::socket socket_;
	enum { max_length = 1024, max_blocksize = 65536 };
	char data [max_length];

	std::chrono::time_point<std::chrono::system_clock> start_time;
};

} // namespace io

#endif