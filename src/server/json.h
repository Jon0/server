#pragma once

#include <unordered_map>

#include "http.h"
#include "json11.h"

namespace json {

template<class T>
class json_static : public http::static_content {
public:
	json_static(T const *object)
		:
		http::static_content(to_json(*object).dump(), "application/json") {
	}

};

template<class T>
class json_live : public http::live_content {
public:
	json_live(std::function<T const *()> object)
		:
		http::live_content(to_json(*object()).dump(), "application/json") {
	}

};

}