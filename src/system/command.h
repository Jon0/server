#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class CommandLine {
public:
	CommandLine() {}

	std::string exec(std::string);

};

#endif