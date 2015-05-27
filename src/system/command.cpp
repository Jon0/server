#include <iostream>

#include "command.h"

bool CommandLine::has_command(const std::string &cmd_name) {
	auto ac = available.find(cmd_name);
	if (ac == available.end()) {
		std::cout << cmd_name << " check\n";

		// command has not been previously used
		auto r = get_output("which "+cmd_name);
		bool has_cmd = r.length() > 0;
		available[cmd_name] = has_cmd;

		if (!has_cmd) {
			std::cout << "command " << cmd_name << " not available\n";
		}
		else {
			std::cout << cmd_name << " @ " << r << "\n";
		}
		return has_cmd;
	}
	return ac->second;
}

std::string CommandLine::exec(std::string cmd, std::vector<std::string> args) {
	bool a = has_command(cmd);
	if (!a) {
		return "";
	}

	// run the command with args
	std::string full_cmd = cmd;
	for (auto &a: args) {
		cmd += " " + a;
	}
	return get_output(full_cmd);
}

std::string CommandLine::exec(std::string cmd) {
	bool a = has_command(cmd);
	if (!a) {
		return "";
	}

	// run the command
	return get_output(cmd);
}

std::string CommandLine::get_output(std::string cmd) {

	FILE* pipe = popen(cmd.c_str(), "r");
	if (!pipe) return "ERROR";

	// copy output to a std::string
	char buffer[128];
	std::string result = "";

	while(!feof(pipe)) {
		if(fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}

	pclose(pipe);
	return result;
}
