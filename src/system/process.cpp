#include <iostream>
#include <vector>

#include "../parser.h"
#include "command.h"
#include "process.h"

namespace sys {

SambaMonitor::SambaMonitor() {

}

void SambaMonitor::update() {
	CommandLine cl;
	auto smbstatus = cl.exec("smbstatus");
	auto lines = io::split(smbstatus, '\n');
	
	std::cout << lines[3] << std::endl;
	active = !(lines[3].length() == 0);
}

bool SambaMonitor::active_sessions() {
	return active;
}

std::string SambaMonitor::html() {
	std::string result;
	result += "<h1>";
	if (active) {
		result += "samba active";
	}
	else {
		result += "samba idle";
	}
	result += "</h1>";
	return result;
}

} // namespace sys
