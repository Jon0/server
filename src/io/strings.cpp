#include <fstream>
#include <iostream>

#include <sstream>
#include <stdexcept>
#include <vector>

#include "strings.h"

namespace io {

int parse_int(const std::string &str, int default_value) {
	int result = default_value;
	try {
		result = stoi(str);
	}
	catch (const std::invalid_argument &) {}
	return result;
}

std::vector<std::string> split(const std::string &str, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, delim)) {
		if (item.length() > 0) {
			result.push_back(item);
		}
	}
	return result;
}

std::string read_file(const std::string &filename) {
	std::ifstream t(filename);
	std::stringstream ss;
	ss << t.rdbuf();
	return ss.str();
}

} // namespace io