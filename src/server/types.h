#pragma once

namespace http {

class request;

using connect_t = request;
using connect_func_t = std::function<void(const connect_t &)>;

};
