#pragma once

#include <iterator>
#include <string>
#include <vector>

namespace io {

/**
 * safely read an int value
 */
int parse_int(const std::string &str, int default_value=0);

/**
 * string split function using a char delimiter
 */
std::vector<std::string> split(const std::string &str, char delim);

/**
 * read a whole file into a string
 */
std::string read_file(const std::string &filename);

} // namespace io
