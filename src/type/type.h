#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

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

using val_list = std::vector<const value_base *>;
using type_func = std::function<bool(value_base &modify, const val_list &args)>;


class func {
public:
	func(const std::string &name, const type_func f);

	/**
	 * pass mouse position
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


/**
 * stores a type and value
 */
class typed_value {
public:

	// create a new typed value
	template<class T>
	static typed_value create(value<T> &initial) {
		return typed_value(*atomic<T>::get(), initial);
	}

	/**
	 * check equality
	 */
	bool operator ==(const typed_value &other) const;

	/**
	 * assignment
	 */
	const typed_value &operator =(const typed_value &other);

	/**
	 * type converted to a string
	 */
	std::string str_value() const;

	/**
	 * read inner type - the templated type must match
	 */
	template<class T>
	const T &get_value() const {
		return var->get<T>();
	}

	const type_base &type;

private:
	typed_value(const type_base &ty, value_base &val);

	/**
	 * set the value
	 */
	void set(const typed_value &other);

	template<class T>
	void set_value(const typed_value &other) {
		const T &other_value = other.get_value<T>();
		if (this->var) {
			this->var->set<T>(other_value);
		}
		else {
			//this->var = std::make_unique<value<T>>(other_value);
		}
	}

	value_base *var;

};

/**
 * Allows objects to expose customisable features
 *
 * This is used to bind inner game classes
 * with console interaction or gui elements
 */
using object = std::unordered_map<std::string, typed_value>;


class node {
public:
	node(const std::string &panel_name);
	virtual ~node();

	/**
	 * lists all available options in a readable format
	 */
	std::vector<std::string> list_options(bool recurse=false, std::string indent="");

	/**
	 * shows all available variable names
	 */
	std::vector<std::string> list_variables() const;

	typed_value *get_variable_rw(const std::string &name);
	const typed_value *get_variable(const std::string &name) const;

	/**
	 * shortcut for get_variable(name).value<T>()
	 */
	template<class T>
	const T &getv(const std::string &name) {
		return this->get_variable(name)->get_value<T>();
	}

	/**
	 * return a child by name, or null if not available
	 */
	node *get_child(const std::string &name) const;

	/**
	 * Set a parent panel which
	 * implies adding a child to the parent
	 */
	void set_parent(node *parent);

	/**
	 * name of this node
	 */
	const std::string name;

	/**
	 * add types to the interface
	 */
	void add(const std::string &vname, const typed_value &value);

private:

	/**
	 * TODO typed_value -> typed_value map?
	 */
	object varmap;

	/**
	 * no longer used
	 */
	void add_node(node *child);
	void remove_node(node *child);
	node *parent;
	std::unordered_map<std::string, node *> nodes;
};


} // namespace type
