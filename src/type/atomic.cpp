#include "atomic.h"

namespace type {

// a map using unique string names for types would be better
type_base *atomic<int>::instance = new atomic<int>();
type_base *atomic<std::string>::instance = new atomic<std::string>();

}
