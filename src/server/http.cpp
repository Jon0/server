#include <thread>
#include <string>

#include "../io/strings.h"
#include "http.h"

namespace http {

header::header() {

}

request::request(request_type t, std::string l)
	:
	type(t),
	location(l) {
}

std::string request::info() const {
	std::string result;
	result += "location : " + location + "\n";
	result += "from name : " + from.name() + "\n";
	for (auto &pair : data) {
		result += pair.first + " : " + pair.second + "\n";
	}
	result += raw_request;
	return result;
}

response_connector::response_connector(const request &req, const content &c)
	:
	source_request(req),
	connected_content(c) {
	connected_content.connect(source_request);
}

response_connector::~response_connector() {
	connected_content.disconnect(source_request);
}

response::response(unsigned short code)
	:
	response_code(code),
	ct("") {
}

response::response(const content &c)
	:
	response_code(200),
	ct(c),
	connector(nullptr) {
	buffer = ct.pull();
}

response::~response() {
	if (ct.is_stream) {
		ct.remove_write_func(write_func);
	}
}

std::string response::header() const {
	std::string header = "";
	header += "HTTP/1.1 " + std::to_string(response_code) + " ";
	header += code_str.at(response_code) + "\n";
	if (!ct.type.empty()) {
		header += "Content-Type: " + ct.type + "\n";
	}
	if (!session_token.empty()) {
		header += "Set-Cookie: sessionToken=" + session_token + "\n";
	}
	if (ct.is_stream) {
		header += "Connection: keep-alive\n";
		header += "Cache-Control: no-cache\n";
		header += "retry: 50000\n";
	}
	else {
		header += "Content-Length: " + std::to_string(buffer.size()) + "\n";
	}
	header += "\n";
	return header;
}

void response::include_token(const std::string &t) {
	session_token = t;
}

void response::send(const request &req, write_func_t write) {
	write_func = std::make_shared<write_func_t>(write);

	// TODO lock stream here
	if (ct.is_stream) {
		ct.add_write_func(write_func);
	}

	// header gets written first
	// buffer is empty for streams
	(*write_func)(header() + buffer);

	// should be after adding write function
	connector = std::make_shared<response_connector>(req, ct);
}

request parse_request(const std::string &request_data) {
	auto lines = io::split(request_data, '\n');

	request_type type;
	std::string location;
	str_map data;

	// parse lines in the request
	bool read_data = false;
	for (auto &line : lines) {
		if (read_data) {
			// read request data past the header
			auto items = io::split(line, '&');
			for (auto &item : items) {
				auto key_value = io::split(item, '=');
				if (key_value.size() >= 2) {
					data[key_value[0]] = key_value[1];
				}
			}
		}
		else {
			// read http header
			auto items = io::split(line, ' ');

			// if line starts with an http verb
			if (items.size() > 1 && request_str.count(items[0]) > 0) {

				// parse verb type
				type = request_str.at(items[0]);

				// look for extra components
				auto components = io::split(items[1], '?');
				location = components[0];

				// parse url arguments
				if (components.size() > 1) {
					auto key_value = io::split(components[1], '=');
					if (key_value.size() >= 2) {
						data[key_value[0]] = key_value[1];
					}
				}
			}
			else if (items.size() > 1 && items[0] == "Cookie:") {
				auto cookies = io::split(items[1], '\r');
				for (auto &c : cookies) {
					auto key_value = io::split(c, '=');
					if (key_value.size() >= 2) {
						data[key_value[0]] = key_value[1];
					}
				}
			}
			else if (items.size() == 1) {
				// end of http header
				read_data = true;
			}
		}
	}

	// create and return request
	request req(type, location);
	req.data = data;
	req.raw_request = request_data;
	return req;
}

}
