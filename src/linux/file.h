#pragma once

#include "../type/var.h"

namespace os {

/**
 * A file directory
 */
class directory : public type::node {
public:
	directory(const std::string &path);
	virtual ~directory();

	/**
	 * read all files with a given extension
	 */
	std::vector<std::string> file_list();

private:
	type::var<std::string> path;
	type::var<std::vector<std::string>> files;

};

}
