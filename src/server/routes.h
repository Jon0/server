#pragma once

#include <functional>
#include <regex>
#include <unordered_map>

#include "http.h"

namespace http {

using route_args_t = std::vector<std::string>;
using content_func_t = std::function<http::content(const route_args_t &, const request &request)>;
using notify_func_t = std::function<void(const request &)>;

/**
 * default types for loaded files
 */
const std::unordered_map<std::string, std::string> ext_type = {
	{"html", "text/html"},
	{"js", "application/javascript"},
	{"vert", "text/plain"},
	{"frag", "text/plain"}
};

class route {
public:
	route(const std::string &s);

	std::string get_type() const;

	bool match(const std::string &s, route_args_t &args) const;

private:
	route_args_t components;

};

/**
 * TODO routes matched by regex
 */
class routes {
public:
	routes(const std::string &root);

	response get_response(const request &request) const;

	std::string get_type(const std::string &filename) const;

	/**
	 * add a regex matched set of routes
	 */
	void add_routes(const std::string &uri, content_func_t ct);

	/**
	 * serve a single content object
	 */
	void add_location(const std::string &uri, content *ct);

	void add_notify_func(const std::string &uri, notify_func_t func);

private:
	std::string web_root;
	std::unordered_map<std::string, std::string> alias;

	// dynamic locations matched with regex
	// this is less optimised
	std::vector<std::pair<route, content_func_t>> dynamic_routes;

	// routes which return content
	std::unordered_map<std::string, http::content *> static_routes;

	// routes which notify
	std::unordered_map<std::string, notify_func_t> notify;
};

}
