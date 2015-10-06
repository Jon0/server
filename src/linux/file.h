#pragma once

#include <string>
#include <vector>

namespace os {

/**
 * A file directory
 */
class directory {
public:
	directory(const std::string &path);
	virtual ~directory();

	/**
	 * read all files with a given extension
	 */
	std::vector<std::string> file_list();

private:
	std::string path;

};

}
