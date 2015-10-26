#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "content.h"
#include "user.h"

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

const std::unordered_map<unsigned short, std::string> code_str = {
	{200, "OK"},
	{404, "Not Found"}
};

/**
 * http header
 */
class header {
public:
	header();
};

class request: protected header {
public:
	request(request_type t, std::string l);

	const request_type type;

	// requested url
	const std::string location;

	// session cookie
	server::user from;

	// ip and port of remote
	std::string remote;

	// additional variables
	str_map data;

	// split request content
	std::vector<std::string> split;

	// unparsed request for debug
	std::string raw_request;

	std::string info() const;

};

/**
 * provides connect and disconnect callback messages
 * to the given content
 */
class response_connector {
public:
	response_connector(const request &req, const content &c);
	~response_connector();

private:
	request source_request;
	content connected_content;
};

/**
 * response to an http request
 */
class response {
public:
	response(const request &req, unsigned short code);

	/**
	 * Makes a single pull() on content
	 * callbacks are setup if content is a stream
	 */
	response(const request &req, const content &c);
	~response();

	std::string header() const;

	void include_token(const std::string &);

	// non-blocking
	void send(const request &req, write_func_t write);

	const unsigned short response_code;

private:
	content ct;
	std::string session_token;
	std::shared_ptr<response_connector> connector;
	std::shared_ptr<write_func_t> write_func;

	// temp read buffer
	std::string buffer;

};

/**
 * parsing an http request header and data
 */
request parse_request(const std::string &request_data);

}
