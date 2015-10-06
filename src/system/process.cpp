#include <iostream>
#include <vector>

#include "../io/strings.h"
#include "../linux/command.h"
#include "process.h"
#include "system.h"

namespace sys {

SambaMonitor::SambaMonitor() {

}

bool SambaMonitor::available() const {
	return System::get()->cl.has_command("smbstatus");
}

void SambaMonitor::update() {
	std::cout << "smdstatus\n";
	auto smbstatus = System::get()->cl.exec("smbstatus");
	auto lines = io::split(smbstatus, '\n');

	int as = 0;
	connections.clear();
	for (int i = 3; i < lines.size() && lines[i].substr(0, 7) != "Service"; ++i) {
		//std::cout << "Session: " << lines[i] << std::endl;
		connections.push_back(lines[i]);
		as++;
	}
	active = as;
}

int SambaMonitor::active_sessions() {
	return active;
}

std::string SambaMonitor::html() const {
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

bool MythTvMonitor::available() const {
	return System::get()->cl.has_command("mythtv-status");
}

void MythTvMonitor::update() {
	std::cout << "mstatus\n";
	auto smbstatus = System::get()->cl.exec("mythtv-status");
	auto lines = io::split(smbstatus, '\n');

	int as = 0;
	connections.clear();
	for (int i = 5; i < lines.size() && lines[i].substr(0, 8) != "Schedule"; ++i) {
		//std::cout << "Session: " << lines[i] << std::endl;
		connections.push_back(lines[i]);
		as++;
	}
	active = as;
}

int MythTvMonitor::active_sessions() {
	return active;
}

std::string MythTvMonitor::html() const {
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
