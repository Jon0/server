#include "http.h"

namespace http {

header::header() {

}

request::request(request_type t, std::string l)
	:
	type(t),
	location(l) {

}

response::response(unsigned short code)
	:
	response_code(code),
	response_content() {

}

response::response(std::string content)
	:
	response_code(200),
	response_content(content) {

}

std::string response::as_str() {
	return "";
}

} 
