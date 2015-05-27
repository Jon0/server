#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <unordered_map>
#include <vector>

enum class command {
	shutdown
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

#endif