#include <iostream>

#include "../io/strings.h"
#include "json.h"
#include "routes.h"

namespace http {

route::route(const std::string &s) {
	for (auto &c : io::split(s, '/')) {
		components.push_back(c);
	}
}

std::string route::get_type() const {
	auto parts = io::split(components.back(), '.');
	if (parts.size() == 2 && ext_type.count(parts[1]) > 0) {
		return ext_type.at(parts[1]);
	}

	// default type;
	return "text/html";
}

bool route::match(const std::string &s, route_args_t &args) const {
	auto other = io::split(s, '/');

	// must be the same length
	if (components.size() != other.size()) {
		return false;
	}

	// match each component
	for (int i = 0; i < components.size(); ++i) {
		const std::string &c = components[i];
		const std::string &o = other[i];
		if (c == "*") {
			args.push_back(o);
		}
		else if (c != o) {
			return false;
		}
	}
	return true;
}

routes::routes(const std::string &root)
	:
	web_root(root),
	alias({{"/", "/index.html"}}) {
}

response routes::get_response(const request &request) const {
	std::string uri;
	if (alias.count(request.location) > 0) {
		uri = alias.at(request.location);
	}
	else {
		uri = request.location;
	}

	// respond to notifiers if available
	bool do_notify = (notify.count(uri) > 0);
	if (do_notify) {
		notify.at(uri)(request);
	}

	// return static mapped content
	if (static_routes.count(uri) > 0) {
		return response(*static_routes.at(uri));
	}

	// return dynamic content as a lower priority
	for (auto &r : dynamic_routes) {
		route_args_t args;
		if (r.first.match(uri, args)) {
			return response(r.second(args, request));
		}
	}

	// try read the files
	std::string full_path = web_root + uri;
	std::cout << "reading file " << full_path << "\n";
	std::string file = io::read_file(full_path);
	if (!file.empty()) {
		static_content content(file, get_type(uri));
		return response(content);
	}
	else if (do_notify) {

		// notify routes will not 404
		// but instead return default response
		static_content content("OK", "text/plain");
		return response(content);
	}
	else {
		return response(404);
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
	dynamic_routes.emplace_back(std::make_pair(route(uri), ct));
}

void routes::add_location(const std::string &uri, content *ct) {
	static_routes.emplace(std::make_pair(uri, ct));
}

void routes::add_notify_func(const std::string &uri, notify_func_t func) {
	notify.emplace(std::make_pair(uri, func));
}

}
