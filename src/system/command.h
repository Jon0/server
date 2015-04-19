#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class CommandLine {
public:
	CommandLine() {}

	/**
	 * true if the command is available on this system
	 */
	bool has_command(const std::string &cmd_name);

	std::string exec(std::string);

};

#endif