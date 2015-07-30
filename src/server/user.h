#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace server {

class session;


struct user_data {
	std::string token, name;

	// all sessions using this token
	// map remote names to sessions
	std::unordered_map<std::string, std::weak_ptr<session>> live;

	// list of past access locations
	std::unordered_set<std::string> locations;

	// log additional messages
	std::string log;
};

class user {
public:
	user();
	user(const std::string &t, const std::string &n);

	bool valid() const;
	unsigned int level() const;
	std::string token() const;
	std::string name() const;
	std::string info() const;

	void link_session(std::shared_ptr<session> s);
	void unlink_session(std::shared_ptr<session> s);

	void log(const std::string &msg);

private:
	std::shared_ptr<user_data> data;

};

/**
 * should save this to a file or database
 */
class user_set {
public:
	user_set();

	// user by name
	user by_name(const std::string &name) const;

	// user by token
	user by_token(const std::string &token) const;

	// user by token or a new user
	user new_user(const std::string &token);

	// create new user
	user new_user();

private:
	std::string new_token();
	std::string new_name();

	// name -> user
	std::unordered_map<std::string, user> names;

	// token -> user
	std::unordered_map<std::string, user> tokens;

	int number;

};


} // namespace server
