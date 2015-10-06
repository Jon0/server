#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "atomic.h"

namespace type {

/**
 * A thing to own named values
 */
template<class T>
class var {
public:
	var(const std::string &name, const T &init, node *owner = nullptr)
		:
		name(name),
		val(std::make_shared<value_owned<T>>(init)) {

		// add to owner
		if (owner) {
			owner->add(name, typed());
		}
	}

	void set(const T &v) {
		val->set(v);
	}

	const T &get() const {
		return val->get();
	}

	T &access() {
		return val->access();
	}

	typed_value typed() {
		return typed_value::create(val);
	}

	std::string status() {
		typed_value tv = typed();
		return tv.type.name + " " + name + " = " + tv.str_value();
	}

	const std::string name;
	std::shared_ptr<value_owned<T>> val;
};

}
