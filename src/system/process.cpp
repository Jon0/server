#include <vector>
#include <iostream>

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
	connections.clear();
	for (int i = 3; lines[i].substr(0, 7) != "Service"; ++i) {
		//std::cout << "Session: " << lines[i] << std::endl;
		connections.push_back(lines[i]);
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
	result += std::to_string(active) + " Samba connections";
	result += "</h1>";
	for (auto &l: connections) {
		result += "<p>";
		result += l;
		result += "</p>";

	}
	return result;
}

MythTvMonitor::MythTvMonitor() {

}

void MythTvMonitor::update() {
	CommandLine cl;
	auto smbstatus = cl.exec("mythtv-status");
	auto lines = io::split(smbstatus, '\n');

	int as = 0;
	connections.clear();
	for (int i = 5; lines[i].substr(0, 8) != "Schedule" && i < lines.size(); ++i) {
		std::cout << "Session: " << lines[i] << std::endl;
		connections.push_back(lines[i]);
		as++;
	}
	active = as;
}

int MythTvMonitor::active_sessions() {
	return active;
}

std::string MythTvMonitor::html() {
	std::string result;
	result += "<h1>";
	result += std::to_string(active) + " MythTv connections";
	result += "</h1>";
	for (auto &l: connections) {
		result += "<p>";
		result += l;
		result += "</p>";

	}
	return result;
}

} // namespace sys
