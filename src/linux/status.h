#pragma once

#include <string>

#include "../type/var.h"

namespace os {

class status : public type::node {
public:
	status();

private:
	type::var<int> pid;
	type::var<std::string> login;

};

}
