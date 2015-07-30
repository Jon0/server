#include <fstream>
#include <iostream>

#include "../io/strings.h"
#include "log.h"
#include "config.h"

namespace sys {

Config::Config()
	:
	Config("/etc/", "web_pw.conf") {
}

Config::Config(std::string path, std::string file)
	:
	fullpath(path + file) {

	// read config file pairs
	std::ifstream config(fullpath);
	if (!config.is_open()) {
		log() << "Config file not found: " << path << "\n";

		// try the file in the current directory
		fullpath = file;
		config.open(fullpath);
	}


	log() << "Read config file " << fullpath << "\n";
	std::string line;
	while (std::getline(config, line)) {
		auto parts = io::split(line, ':');
		if (parts.size() == 2) {
			items[parts[0]] = parts[1];
			std::cout << parts[0] << " = " << parts[1] << std::endl;
		}
	}
	log() << "Done\n";
}

std::string Config::filepath() const {
	return fullpath;
}

bool Config::has(std::string key) {
	return items.count(key) > 0;
}

std::string Config::get(std::string key) {
	if (items.count(key) == 0) {
		log() << "Config key not found: " << key << "\n";
		return "";
	}
	return items[key];
}

void Config::set(std::string key, std::string value) {
	items[key] = value;
	update_file();
}

void Config::update_file() {

	// update config file
	log() << "Updating config file\n";
	std::ofstream config(fullpath);
	if (config.is_open()) {
		for (auto kv: items) {
			config << kv.first << ":" << kv.second << "\n";
		}
	}
}

} // namespace sys
