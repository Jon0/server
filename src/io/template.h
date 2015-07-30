#pragma once

#include <string>
#include <vector>

#include "../type/type.h"

namespace io {

struct tmpl_item {
	bool replace;
	std::string content;
};

class tmpl {
public:
	tmpl(const std::string &in);

	std::string dump() const;

	std::string render(const type::node &node) const;

private:
	void add_item(const std::string &content, bool replace);

	std::vector<tmpl_item> items;


};

}
