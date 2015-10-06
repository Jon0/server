#pragma once

#include <string>
#include <unordered_map>
#include <vector>


enum class cmd_name {
	shutdown
};


template<cmd_name cmd>
struct command;

template<>
struct command<cmd_name::shutdown> {
	int call(int time) {
		// TODO
	}
};




class CommandLine {
public:
	CommandLine() {}

	/**
	 * true if the command is available on this system
	 */
	bool has_command(const std::string &cmd_name);


	std::string exec(std::string cmd, std::vector<std::string> args);
	std::string exec(std::string cmd);

private:
	std::unordered_map<std::string, bool> available;


	std::string get_output(std::string cmd);

};
