#include <regex>

#include "template.h"

namespace io {

tmpl::tmpl(const std::string &in) {
	std::string s = in;
	std::smatch om;
	std::regex open("\\{{2}");
	std::regex close("\\}{2}");

	while (std::regex_search(s, om, open)) {
		std::smatch cm;
		std::string sf = om.suffix().str();
		if (std::regex_search(sf, cm, close)) {
			add_item(om.prefix().str(), false);
			add_item(cm.prefix().str(), true);
			s = cm.suffix().str();
		}
		else {
			break;
		}
	}
	add_item(s, false);
}

std::string tmpl::dump() const {
	std::string result = "";
	for (auto &i : items) {
		if (i.replace) {
			result += "${" + i.content + "}";
		}
		else {
			result += i.content;
		}
	}
	return result;
}

std::string tmpl::render(const type::node &node) const {
	std::string result = "";
	for (auto &i : items) {
		if (i.replace) {
			const type::typed_value *tv = node.get_variable(i.content);
			if (tv) {
				result += tv->str_value();
			}
		}
		else {
			result += i.content;
		}
	}
	return result;
}

void tmpl::add_item(const std::string &content, bool replace) {
	if (!content.empty()) {
		items.emplace_back(tmpl_item{replace, content});
	}
}

}
