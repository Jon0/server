#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>

namespace sys {


/**
 * read and write key value pairs to the config file
 */
class Config {
public:
	Config();
	Config(std::string path, std::string file);

	std::string filepath() const;

	/**
	 * does the given key have a value
	 */
	bool has(std::string key);

	/**
	 * returns value associated with the given key
	 */
	std::string get(std::string key);

	/**
	 * modifies the value and updates the config file
	 */
	void set(std::string key, std::string value);

private:
	std::string fullpath;
	std::unordered_map<std::string, std::string> items;

	void update_file();

};

} // namespace sys

#endif