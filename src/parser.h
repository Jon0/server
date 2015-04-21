#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "http.h"

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
 * parsing an http request header and data
 */
http::request parse_request(const char *request_data, int length);

} // namespace io

#endif