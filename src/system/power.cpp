#include <iostream>
#include <vector>

#include "../parser.h"
#include "power.h"
#include "system.h"

namespace sys {

PowerCtrl::PowerCtrl() {
	idle_sec = 0;
	idle_shutdown_sec = 15 * 60;
	start = std::chrono::system_clock::now();
}

void PowerCtrl::update() {

	// sec since startup is the maximum run time
	std::chrono::duration<double> dur = std::chrono::system_clock::now() - start;
	int sec_since_start = dur.count();
	auto w_out = cl.exec("w");
	auto lines = io::split(w_out, '\n');
	int new_idle_time = sec_since_start;

	// parse the lines from w
	for (int i = 2; i < lines.size(); ++i) {
		auto tokens = io::split(lines[i], ' ');
		auto idle_str = tokens[4];
		auto last_char = idle_str[idle_str.length() - 1];
		int sum_time = sec_since_start;
		if (last_char == 'm') {
			auto time_parts = io::split(idle_str.substr(0, idle_str.length() - 1), ':');
			sum_time = stoi(time_parts[0]) * 60 * 60 + stoi(time_parts[1]) * 60;
			std::cout << time_parts[0] << " hour " << time_parts[1] << " min " << std::endl;
		}
		else if (last_char == 's') {
			auto time_parts = io::split(idle_str.substr(0, idle_str.length() - 1), '.');
			sum_time = stoi(time_parts[0]);
			std::cout << time_parts[0] << " sec " << std::endl;
		}
		else {
			auto time_parts = io::split(idle_str, ':');
			sum_time = stoi(time_parts[0]) * 60 + stoi(time_parts[1]);
			std::cout << time_parts[0] << " min " << time_parts[1] << " sec " << std::endl;
		}

		// record for each user
		activity[tokens[0]] = sum_time;

		// find lowest value
		if (sum_time < new_idle_time) {
			new_idle_time = sum_time;
		}


	}

	// look for x activity too
	auto xidle = cl.exec("xprintidle");
	if (isdigit(xidle[0])) {
		int sum_time = stoi(xidle) / 1000;
		activity["x"] = sum_time;
		if (sum_time < new_idle_time) {
			new_idle_time = sum_time;
		}
	}

	// check samba
	if (System::get()->get_samba()->active_sessions()) {
		new_idle_time = 0;
	}

	idle_sec = new_idle_time;
	std::cout << "idle for " << idle_sec << " sec" << std::endl;
	if (idle_sec > idle_shutdown_sec) {
		std::cout << "system shuting down now" << std::endl;
		shutdown();
	}
}

int PowerCtrl::idle_seconds() {
	return idle_sec;
}

int PowerCtrl::idle_shutdown_seconds() {
	return idle_shutdown_sec;
}

std::string PowerCtrl::html() {
	std::string result;
	result += "<p>active users</p>";
	result += "<table>";
	result += "<tr>";
	for (auto u: activity) {
		result += "<td>"+u.first+"</td>";
		result += "<td>"+std::to_string(u.second)+"</td>";
	}
	result += "</tr>";
	result += "</table>";
	return result;
}

void PowerCtrl::shutdown() {
	cl.exec("ethtool -s eth1 wol g");
	cl.exec("shutdown -P 0");
}

} // namespace sys
