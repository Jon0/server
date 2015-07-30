#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#include "types.h"

namespace http {

using read_func_t = std::function<std::string()>;
using write_func_t = std::function<void(const std::string &)>;

struct content_data {

	// a mutex gets locked for all operations
	std::mutex write_mutex, connect_mutex;

	// buffering when not streaming
	// read function takes priority
	std::stringstream ss;
	read_func_t read;

	// functions to write data stream to
	std::vector<std::weak_ptr<write_func_t>> write;

	// notifying on connect and disconnect
	connect_func_t connect_func, disconnect_func;
};

/**
 * generic content
 */
class content {
public:
	content(std::string type, bool stream = false);

	const std::string type;
	const bool is_stream;

	virtual std::string pull() const;

	/**
	 * push directly to the write function if
	 * available, otherwise adding to the stream
	 * returned by pull
	 */
	void push(const std::string &str) const;

	// callback functions
	void on_connect(connect_func_t f);
	void on_disconnect(connect_func_t f);

	// sends to callbacks
	void connect(const connect_t &);
	void disconnect(const connect_t &);

protected:
	void set_read_func(read_func_t f);
	void add_write_func(std::shared_ptr<write_func_t> f);
	void remove_write_func(std::shared_ptr<write_func_t> f);

private:
	std::shared_ptr<content_data> data;

	friend class response;
};

/**
 * fixed length content
 */
class static_content : public content {
public:
	static_content(std::string data_str, std::string type);

};

/**
 * function which returns content
 */
class live_content : public content {
public:
	live_content(read_func_t f, std::string type);

};

/**
 * html5 stream content
 */
class stream : public content {
public:
	stream();

	void message(const std::string &str);
	void message(const content &content);

};

} // namespace http
