#include <iostream>

#include "../io/strings.h"
#include "json.h"
#include "routes.h"

namespace http {

route_tree::route_tree() {}

void route_tree::insert(const split_type &nodes, content_func_t ct) {
	if (nodes.empty()) {
		content = ct;
		return;
	}
	auto next_node = make_node(nodes.front());
	auto subset = split_type(nodes.begin() + 1, nodes.end());
	next_node->insert(subset, ct);
}

content_func_t route_tree::match(const split_type &nodes) {
	if (nodes.empty()) {
		return content;
	}
	for (auto &r : next) {
		try {
			if (std::regex_match(nodes.front(), std::regex(r.first))) {
				auto subset = split_type(nodes.begin() + 1, nodes.end());
				auto branch = r.second->match(subset);
				if (branch) {
					return branch;
				}
			}
		} catch(std::regex_error const &e) {
			std::cout << e.what() << "\n";
		}
	}
	//std::cout << "could not match " << nodes.front() << "\n";
	return nullptr;
}

std::shared_ptr<route_tree> route_tree::make_node(const regex_type &str) {
	for (auto &pair : next) {
		if (str == pair.first) {
			return pair.second;
		}
	}
	std::shared_ptr<route_tree> new_route = std::make_shared<route_tree>();
	next.emplace_back(std::make_pair(str, new_route));
	return new_route;
}

routes::routes()
	:
	root(std::make_shared<route_tree>()) {}

response routes::get_response(const request &request) const {
	std::string uri = request.location;

	// respond to notifiers if available
	bool do_notify = (notify.count(uri) > 0);
	if (do_notify) {
		notify.at(uri)(request);
	}

	// return content
	auto content = root->match(request.split);
	if (content) {
		return response(request, content(request));
	}

	if (do_notify) {

		// notify routes will not 404
		// but instead return default response
		static_content content("OK", "text/plain");
		return response(request, content);
	}
	else {
		return response(request, 404);
	}
}

std::string routes::get_type(const std::string &filename) const {
	auto components = io::split(filename, '.');
	auto size = components.size();
	if (size > 0) {
		// the string following the last '.'
		auto last = components[size - 1];
		auto type = ext_type.find(last);
		if (type != ext_type.end()) {
			return type->second;
		}
	}
	return "text/html";
}

void routes::add_routes(const std::string &uri, content_func_t ct) {
	root->insert(io::split(uri, '/'), ct);
}

void routes::add_notify_func(const std::string &uri, notify_func_t func) {
	notify.emplace(std::make_pair(uri, func));
}
}
