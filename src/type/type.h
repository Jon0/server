#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "base.h"

namespace type {

/**
 * stores a type and value
 */
class typed_value {
public:
	typed_value(const typed_value &tv);

	// create a new typed value
	template<class T>
	static typed_value create(const value_shared<T> &initial) {
		return typed_value(*atomic<T>::get(), std::make_shared<value_shared<T>>(initial));
	}

	// create a new typed value
	template<class T>
	static typed_value create(const T &initial) {
		return typed_value(*atomic<T>::get(), std::make_shared<value_owned<T>>(initial));
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

	typed_value func(const std::string &fname, const val_list &args);

	const type_base &type;

private:
	typed_value(const type_base &ty, std::shared_ptr<value_base> val);

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

	std::shared_ptr<value_base> var;
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
