#pragma once

#include <functional>
#include <regex>
#include <unordered_map>

#include "http.h"

namespace http {

using route_args_t = std::vector<std::string>;
using content_func_t = std::function<http::content(const request &)>;
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

class route_tree {
public:
	using regex_type = std::string;
	using split_type = std::vector<regex_type>;
	using value_type = std::pair<regex_type, std::shared_ptr<route_tree>>;

	route_tree();

	void insert(const split_type &nodes, content_func_t ct);

	content_func_t match(const split_type &nodes);

	std::shared_ptr<route_tree> make_node(const regex_type &str);

private:
	content_func_t content;
	std::vector<value_type> next;
};


/**
 * routes matched by regex
 */
class routes {
public:
	routes();

	response get_response(const request &request) const;

	std::string get_type(const std::string &filename) const;

	/**
	 * add a regex matched set of routes
	 */
	void add_routes(const std::string &uri, content_func_t ct);

	void add_notify_func(const std::string &uri, notify_func_t func);

private:
	std::shared_ptr<route_tree> root;

	// routes which notify
	std::unordered_map<std::string, notify_func_t> notify;
};

}
