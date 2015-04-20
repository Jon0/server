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
	
	int as = 0;
	for (int i = 3; lines[i].substr(0, 7) != "Service"; ++i) {
		std::cout << "Session: " << lines[i] << std::endl;
		as++;
	}
	active = as;
}

int SambaMonitor::active_sessions() {
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
