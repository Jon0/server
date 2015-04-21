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
	Config(std::string filepath);

	std::string get(std::string key);
	void set(std::string key, std::string value);

private:
	std::string path;
	std::unordered_map<std::string, std::string> items;

	void update_file();

};

} // namespace sys

#endif