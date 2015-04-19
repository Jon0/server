#ifndef HTTP_H
#define HTTP_H

#include <string>
#include <unordered_map>

namespace http {

using str_map = std::unordered_map<std::string, std::string>;

/**
 * standard http request verbs
 */
enum class request_type {
	http_get,
	http_post,
	http_put,
	http_delete
};

const std::unordered_map<std::string, request_type> request_str = {
	{"GET", request_type::http_get},
	{"POST", request_type::http_post},
	{"PUT", request_type::http_put},
	{"DELETE", request_type::http_delete},
};	

class header {
public:
	header();
};

class request: protected header {
public:
	request(request_type t, std::string l);

	const request_type type;
	const std::string location;
	str_map data;
};

class response: protected header {
public:
	response(unsigned short code);
	response(std::string content);

	std::string as_str();

	const unsigned short response_code;
	const std::string response_content;
};

}

#endif 
