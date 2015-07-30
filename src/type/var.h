#pragma once

#include <string>

#include "atomic.h"

namespace type {

/**
 * A thing to own values
 */
template<class T>
class var : public value<T> {
public:
	var(const std::string &name, const T &init, node *owner = nullptr)
		:
		value<T>(init),
		name(name) {

		// add to owner
		if (owner) {
			owner->add(name, typed());
		}
	}

	typed_value typed() {
		return typed_value::create(*this);
	}

	std::string status() {
		typed_value tv = typed();
		return tv.type.name + " " + name + " = " + tv.str_value();
	}

	const std::string name;
};

}
