#include <iostream>

#include "content.h"

namespace http {

content::content(std::string type, bool stream)
	:
	type(type),
	is_stream(stream),
	data(std::make_shared<content_data>()) {
}

std::string content::pull() const {
	std::lock_guard<std::mutex> lock(data->write_mutex);
	if (data->read) {
		return data->read();
	}
	return data->ss.str();
}

void content::push(const std::string &str) const {
	std::lock_guard<std::mutex> lock(data->write_mutex);
	if (is_stream) {
		if (data->write.empty()) {
			std::cout << "warning, stream has no outputs\n";
			std::cout << str << "\n\n";
		}

		// write using all open responses
		for (auto &wf : data->write) {
			if (auto sp = wf.lock()) {
				(*sp)(str);
			}
		}
	}
	else {
		data->ss << str;
	}
}

void content::on_connect(connect_func_t f) {
	std::lock_guard<std::mutex> lock(data->connect_mutex);
	data->connect_func = f;
}

void content::on_disconnect(connect_func_t f) {
	std::lock_guard<std::mutex> lock(data->connect_mutex);
	data->disconnect_func = f;
}

void content::connect(const connect_t &c) {
	std::lock_guard<std::mutex> lock(data->connect_mutex);
	if (data->connect_func) {
		data->connect_func(c);
	}
}

void content::disconnect(const connect_t &c) {
	std::lock_guard<std::mutex> lock(data->connect_mutex);
	if (data->disconnect_func) {
		data->disconnect_func(c);
	}
}

void content::set_read_func(read_func_t f) {
	std::lock_guard<std::mutex> lock(data->write_mutex);
	data->read = f;
}

void content::add_write_func(std::shared_ptr<write_func_t> f) {
	std::lock_guard<std::mutex> lock(data->write_mutex);

	// TODO clean up unused functions
	data->write.push_back(f);
}

void content::remove_write_func(std::shared_ptr<write_func_t> f) {
	std::lock_guard<std::mutex> lock(data->write_mutex);

	//data->write.erase();
}


static_content::static_content(std::string data_str, std::string type)
	:
	content(type) {
	push(data_str);
}

live_content::live_content(read_func_t f, std::string type)
	:
	content(type) {
	set_read_func(f);
}

stream::stream()
	:
	content("text/event-stream", true) {
}

void stream::message(const std::string &str) {
	push("data:"+str+"\n\n");
}

void stream::message(const content &content) {
	message(content.pull());
}

} // namespace http
