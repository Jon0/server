#include "command.h"

std::string Test::exec(std::string cmd) {

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
