#include <stdlib.h>
#include <algorithm>

#include "session.h"
#include "user.h"

namespace server {

user::user()
	:
	data(nullptr) {
}

user::user(const std::string &t, const std::string &n)
	:
	data(std::make_shared<user_data>()) {
	data->token = t;
	data->name = n;
}

bool user::valid() const {
	return static_cast<bool>(data);
}


unsigned int user::level() const {
	return 1000;
}


std::string user::token() const {
	if (valid()) {
		return data->token;
	}
	return "";
}

std::string user::name() const {
	if (valid()) {
		return data->name;
	}
	return "";
}

std::string user::info() const {
	if (valid()) {
		std::string str;
		str += "name=" + data->name +"\n";
		str += "token=" + data->token +"\n";
		str += "live=[\n";
		for (auto &l : data->live) {
			str += "\t" + l.first + "\n";
		}
		str += "]\n";
		str += "history=[\n";
		for (auto &l : data->locations) {
			str += "\t" + l + "\n";
		}
		str += "]\n";
		str += "log\n";
		str += data->log;
		return str;
	}
	else {
		return "invalid user";
	}
}

void user::link_session(std::shared_ptr<session> s) {
	if (s && valid()) {
		const std::string &r_name = s->remote_name();
		data->live.emplace(std::make_pair(r_name, s));
		data->locations.emplace(r_name);
	}
}

void user::unlink_session(std::shared_ptr<session> s) {
	if (s && valid()) {
		const std::string &r_name = s->remote_name();
		data->live.erase(r_name);
	}
}

void user::log(const std::string &msg) {
	if (valid()) {
		data->log += "\n" + msg + "\n";
	}
}

user_set::user_set()
	:
	number(0) {
}

user user_set::by_name(const std::string &name) const {
	if (names.count(name) == 0) {
		return user();
	}
	return names.at(name);
}

user user_set::by_token(const std::string &t) const {
	if (tokens.count(t) == 0) {
		return user();
	}
	return tokens.at(t);
}

user user_set::new_user(const std::string &token) {
	if (tokens.count(token) == 0) {
		user u(token, new_name());
		names.emplace(std::make_pair(u.name(), u));
		tokens.emplace(std::make_pair(u.token(), u));
		return u;
	}
	return tokens.at(token);
}

user user_set::new_user() {
	return new_user(new_token());
}

std::string user_set::new_token() {
	auto randchar = []() {
        const char charset[] =
        	"0123456789"
        	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        	"abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };

    size_t length = 40;
    std::string str(length,0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

std::string user_set::new_name() {
	return "lol" + std::to_string(number++);
}

} // namespace server
