#pragma once

#include "value.h"

namespace type {

class typed_value;

template<class T>
class atomic;


/**
 * a possible function implementation
 */
template<class T>
class mod {

};

using val_list = std::vector<const typed_value *>;
using type_func = std::function<bool(value_base &modify, const val_list &args)>;


class func {
public:
	func(const std::string &name, const type_func f);

	/**
	 *
	 */
	bool execute(value_base &modify, const val_list &args);

	/**
	 * name of this action
	 */
	const std::string name;

private:
	const type_func to_exec;

};


/**
 * a runtime type
 */
class type_base {
public:
	type_base(const std::string &name);
	virtual ~type_base() {}

	/**
	 * check equality
	 */
	bool operator ==(const type_base &other) const;
	bool operator !=(const type_base &other) const;

	/**
	 * runs a templated function
	 * using the internal type
	 */
	virtual void tmpl(const value_base &tv) const = 0;

	virtual std::string str_value(const value_base &tv) const = 0;

	virtual void set(value_base &changed, const value_base &value) const = 0;

	void func(const std::string &name, const type_func &f);

	bool execute(const std::string &name, value_base &modify, const val_list &args);

	const std::string name;

private:

	// list available functions for interaction
	std::unordered_map<std::string, type_func> functions;

};

} // type
