#pragma once

#include <vector>

#include "type.h"

namespace type {


/**
 * atomic types
 */
template<class T>
class atomic : public type_base {

	using storage = T;

public:
	atomic()
		:
		type_base("unknown") {}

	void tmpl(const value_base &tv) const override {
		// define for each function
		mod<T> m;
	}

	std::string str_value(const value_base &tv) const override {
		return std::to_string(tv.get<T>());
	}

	value<T> create(const T &initial) {
		return value<T>(initial);
	}

	void set(value_base &changed, const value_base &value) const override {
		changed.set<T>(value.get<T>());
	}

	static type_base *get() {
		return new atomic<T>();
	}
};


template<>
class atomic<int> : public type_base {
	using internal_type = int;
public:
	atomic()
		:
		type_base("int") {

		// functions
		func("add", [](value_base &modify, const val_list &args) {
			for (auto other : args) {
				modify.set<int>(modify.get<int>() + other->get<int>());
			}
			return true;
		});
	}

	void tmpl(const value_base &tv) const override {
		// define for each function
		mod<internal_type> m;
	}

	std::string str_value(const value_base &tv) const override {
		return std::to_string(tv.get<internal_type>());
	}

	value<internal_type> create(const internal_type &initial) {
		return value<internal_type>(initial);
	}

	void set(value_base &changed, const value_base &value) const override {
		changed.set<internal_type>(value.get<internal_type>());
	}

	static type_base *get() {
		return instance;
	}

	static type_base *instance;
};

template<>
class atomic<std::string> : public type_base {
	using internal_type = std::string;
public:
	atomic()
		:
		type_base("string") {
	}

	void tmpl(const value_base &tv) const override {
		// define for each function
		mod<internal_type> m;
	}

	std::string str_value(const value_base &tv) const override {
		return tv.get<internal_type>();
	}

	value<internal_type> create(const internal_type &initial) {
		return value<internal_type>(initial);
	}

	void set(value_base &changed, const value_base &value) const override {
		changed.set<internal_type>(value.get<internal_type>());
	}

	static type_base *get() {
		return instance;
	}

	static type_base *instance;
};

template<class T>
class atomic<std::vector<T>> : public type_base {
	using list_type = std::vector<T>;
	using inner_type = atomic<T>;
public:
	atomic()
		:
		type_base("list<" + inner_type::get()->name + ">") {}

	void tmpl(const value_base &tv) const override {
		// define for each function
		mod<list_type> m;
	}

	std::string str_value(const value_base &tv) const override {
		std::string result = "[";
		bool first = true;
		for (auto &e : tv.get<list_type>()) {
			if (first) {
				result += inner_type::get()->str_value(value<T>(e));
				first = false;
			}
			else {
				result += ", " + inner_type::get()->str_value(value<T>(e));
			}
		}
		result += "]";
		return result;
	}

	value<list_type> create(const list_type &initial) {
		return value<list_type>(initial);
	}

	void set(value_base &changed, const value_base &value) const override {
		changed.set<list_type>(value.get<list_type>());
	}

	static type_base *get() {
		return new atomic<list_type>();
	}
};

/**
 * TODO
 */
template<>
class atomic<type_base> : public type_base {
public:
	atomic()
		:
		type_base("type") {

		// TODO function to list available functions
	}

};

/**
 * TODO use node instead?
 */
template<>
class atomic<object> : public type_base {
	using internal_type = object;
public:
	atomic()
		:
		type_base("object") {}

	void tmpl(const value_base &tv) const override {
		// define for each function
		mod<internal_type> m;
	}

	std::string str_value(const value_base &tv) const override {
		std::string result = "{";
		for (auto &e : tv.get<internal_type>()) {
			result += e.first + " : " + e.second.str_value() + ", ";
		}
		result += "}";
		return result;
	}

	value<internal_type> create(const internal_type &initial) {
		return value<internal_type>(initial);
	}

	void set(value_base &changed, const value_base &value) const override {
		changed.set<internal_type>(value.get<internal_type>());
	}

	static type_base *get() {
		return new atomic<internal_type>();
	}

};



}
