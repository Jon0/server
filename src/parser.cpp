#include <iostream>
#include <sstream>
#include <vector>

#include "parser.h"

namespace io {

std::vector<std::string> split(const std::string &str, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

http::request parse_request(const char *request_data, int length) {
	auto lines = split(std::string(request_data, length), '\n');

	http::request_type type;
	std::string location;
	http::str_map data;

	// parse lines in the request
	bool read_data = false;
	for (auto &line : lines) {
		if (read_data) {
			// read request data
			auto items = split(line, '&');
			for (auto &item : items) {
				auto key_value = split(item, '=');
				if (key_value.size() >= 2) {
					data[key_value[0]] = key_value[1];
				}
			}
		}
		else {
			// read http header
			auto items = split(line, ' ');
			if (items.size() > 1 && http::request_str.count(items[0]) > 0) {
				type = http::request_str.at(items[0]);
				location = items[1];
				std::cout << items[0] << " request for " << location << std::endl;
			}
			else if (items.size() == 1) {
				// end of http header
				read_data = true;
			}
		}
	}

	// create and return request
	http::request req(type, location);
	req.data = data;
	return req;
}

} // namespace io