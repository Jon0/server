#include "system/system.h"
#include "server.h"
#include "session.h"

namespace io {

int session::next_id = 1;

session::session(server &s,
		std::function<void(http::str_map)> &func)
	:
	id(next_id++),
	create_server(s),
	state(session_state::initial),
	write_queue(),
	queue_lock(),
	socket_(s.get_io_service()) {}

session::~session() {}

void session::end() {
	sys::log() << "end session with " << socket().remote_endpoint().address().to_string() 
			<< " (id: " << id << ")\n";

	// clear list of messages to write
	this->queue_lock.lock();
	std::queue<std::string> empty;
	std::swap(this->write_queue, empty);
	this->queue_lock.unlock();

	// end the write thread
	this->state = session_state::stoping;
	try {
		this->write_thread.join();
	}
	catch (...) {
		sys::log() << "Error joining write thread\n";
	}

	// close socket and shutdown
	this->socket().cancel();
	this->state = session_state::stopped;

	// remove from servers list of active clients
	this->create_server.end_session(this);
}

std::string session::description() {
	auto end = std::chrono::high_resolution_clock::now();
	auto sec = std::chrono::duration_cast<std::chrono::seconds>(end - start_time).count();
	return std::to_string(id) + " : " + socket().remote_endpoint().address().to_string() + " ("
		+ std::to_string(sec) + " seconds)";
}

void session::do_read() {
	socket_.async_read_some(boost::asio::buffer(data, max_length),
		[this](boost::system::error_code ec, std::size_t length) {
			if (!ec) {
				sys::log() << "session id: " << id << " -> ";
				auto request = parse_request(data, length);
				if (request.location == "/") {
					auto sys = sys::System::get();
					write_string(sys->html(), "text/html");
				}
				if (request.location == "/idletime") {
					int new_sec = stoi(request.data["minutes"]) * 60;
					auto sys = sys::System::get();
					sys->get_power_ctrl()->set_idle_seconds(new_sec);
					write_string("shutdown time set to "+std::to_string(new_sec), "text/plain");
				}
				else if (request.location == "/off") {
					auto sys = sys::System::get();
					sys->get_power_ctrl()->shutdown();
					write_string("system shut down now", "text/plain");
				}
				else if (request.location == "/stream") {

					// write header and set connection to streaming
					write_stream();
					this->state = session_state::active;
				}
				else if (request.location == "/status") {

					// write status page
					write_string(this->create_server.status(), "text/plain");
				}
				else if (request.type == http::request_type::http_get) {

					// write a page response
					write_page(request.location);
				}
				else if (request.type == http::request_type::http_post) {

					// respond to post headers
					if (create_server.get_update_function()) {
						create_server.get_update_function()(request.data);
					}
					write_string("wot m8", "text/plain");
				}
				sys::log() << "Page delivered\n";

				// read next request in same session
				do_read();
			}
			else if (ec != boost::asio::error::operation_aborted) {
				sys::log() << "Connection closed" << " (id: " << id << ")\n";
				this->end();
			}
			else {
				sys::log() << "Operation aborted\n";
			}
		});
}

void session::write_string(const std::string &str, const std::string &type) {
	std::string header = "";
	header += "HTTP/1.1 200 OK" + newline;
	header += "Content-Type: " + type + newline;
	header += "Cache-Control: no-cache" + newline;
	header += "Content-Length: " + std::to_string(str.size()) + newline;
	header += newline;
	header += str;
	msg(header);
}

void session::write_page(const std::string &filename) {
	std::string content = "";

	std::string file_path = get_location(filename);
	std::string file_type = file_path.substr(file_path.find(".") + 1);
	std::string mime_type;

	// todo use mapping
	if (file_type == "html") {
		mime_type = "text/html";
	}
	else if (file_type == "js") {
		mime_type = "text/javascript";
	}
	else {
		mime_type = "text/plain";
	}

	// open and read file lines
	std::ifstream file(create_server.root_directory() + file_path);
	if (file.is_open()) {
		std::string line;
		while (getline(file, line)) {
			content += line + newline;
		}
	}
	else {
		// todo 404
		std::cout << "cannot open " << file_path << std::endl;
	}

	std::string header = "";
	header += "HTTP/1.1 200 OK" + newline;
	header += "Content-Type: " + mime_type + newline;
	header += "Cache-Control: no-cache" + newline;
	header += "Content-Length: " + std::to_string(content.size()) + newline;
	header += newline;
	header += content;
	msg(header);
}

void session::write_stream() {
	std::string header = "";
	header += "HTTP/1.1 200 OK" + newline;
	header += "Content-Type: text/event-stream" + newline;
	//header += "Transfer-Encoding: chunked" + newline;
	header += "Connection: keep-alive" + newline;
	header += "Cache-Control: no-cache" + newline;
	header += "retry: 15000" + newline;
	header += newline;
	msg(header); // use blocking write here?
}

void session::start_write_thread() {
	write_thread = std::thread([this]() {
		std::mutex write_lock;
		while (true) {

			// ensure a message on top
			while (this->write_queue.empty()) {

				// if stoping then leave
				if (this->state == session_state::stoping) {
					return;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}

			// lock modifying the queue
			this->queue_lock.lock();

			// copy the front item to a container
			auto &str = this->write_queue.front();
			std::vector<char> data;
			std::copy(str.begin(), str.end(), back_inserter(data));

			this->write_queue.pop();
			this->queue_lock.unlock();

			// lock sending on the socket
			write_lock.lock();
			boost::asio::async_write(socket_, boost::asio::buffer(data),
				[&write_lock](boost::system::error_code ec, std::size_t transferred) {
					// todo: queue again if failed to send
					//std::cout << "sent reply (" << transferred << " bytes)" << std::endl;

					// unlocked once message is sent
					write_lock.unlock();
				});
		}
	});
}

} // namespace io