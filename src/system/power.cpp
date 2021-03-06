#include <iostream>
#include <vector>

#include "../io/strings.h"
#include "log.h"
#include "power.h"
#include "system.h"

namespace sys {

PowerCtrl::PowerCtrl()
	:
	idle_sec(0),
	last_update(std::chrono::system_clock::now()) {
}

bool PowerCtrl::available() const {
	return true;
}

void PowerCtrl::update() {

	// sec since last update is the maximum idle time
	auto now = std::chrono::system_clock::now();
	std::chrono::duration<double> dur = now - last_update;
	last_update = now;
	int new_idle_time = idle_sec + dur.count();


	// check w for activity
	new_idle_time = std::min(new_idle_time, w_idle_sec());

	// look for x activity too
	auto xidle = cl.exec("xprintidle");
	if (xidle.length() > 0 && isdigit(xidle[0])) {
		int sum_time = stoi(xidle) / 1000;
		activity["xwindow"] = sum_time;
		new_idle_time = std::min(new_idle_time, sum_time);
	}

	// check samba
	if (System::get()->get_samba()->active_sessions() > 0) {
		new_idle_time = 0;
	}

	// check mythtv
	if (System::get()->get_mythtv()->active_sessions() > 0) {
		new_idle_time = 0;
	}

	idle_sec = new_idle_time;
	if (idle_sec > idle_shutdown_seconds()) {
		log() << "system shuting down now\n";
		shutdown();
	}
}

int PowerCtrl::idle_seconds() const {
	return idle_sec;
}

int PowerCtrl::idle_shutdown_seconds() const {
	auto str_sec = System::get()->get_config()->get("idle_timer_sec");
	int idle_shutdown_sec = io::parse_int(str_sec);
	if (idle_shutdown_sec < 300) {
		idle_shutdown_sec = 300;
	}
	return idle_shutdown_sec;
}

void PowerCtrl::set_idle_seconds(int s) {
	System::get()->get_config()->set("idle_timer_sec", std::to_string(s));
}

std::string PowerCtrl::html() const {
	std::string result;
	result += "<table>";
	result += "<tr>";
	result += "<td>active users</td>";
	result += "<td>idle time</td>";
	result += "</tr>";

	for (auto u: activity) {
		result += "<tr>";
		result += "<td>"+u.first+"</td>";
		result += "<td>"+std::to_string(u.second)+"</td>";
		result += "</tr>";
	}
	result += "</table>";
	result += "<h1>Change idle shutdown minutes</h1>";
	result += "<form action=\"idletime\">";
	result += "<input type=\"text\" name=\"minutes\">";
	result += "<input type=\"submit\" value=\"Set\">";
	result += "</form>";
	return result;
}

void PowerCtrl::shutdown() {
	cl.exec("ethtool -s eth1 wol g");
	cl.exec("shutdown -P 0");
}

int PowerCtrl::w_idle_sec() {
	auto w_out = cl.exec("w");
	auto lines = io::split(w_out, '\n');
	int new_idle_time = 0x0fffffff;

	// parse the lines from w
	for (int i = 2; i < lines.size(); ++i) {
		auto tokens = io::split(lines[i], ' ');
		auto idle_str = tokens[4];
		auto last_char = idle_str[idle_str.length() - 1];
		int sum_time = 0;
		if (last_char == 'm') {
			auto time_parts = io::split(idle_str.substr(0, idle_str.length() - 1), ':');
			sum_time = stoi(time_parts[0]) * 60 * 60 + stoi(time_parts[1]) * 60;
			//std::cout << time_parts[0] << " hour " << time_parts[1] << " min " << std::endl;
		}
		else if (last_char == 's') {
			auto time_parts = io::split(idle_str.substr(0, idle_str.length() - 1), '.');
			sum_time = stoi(time_parts[0]);
			//std::cout << time_parts[0] << " sec " << std::endl;
		}
		else {
			auto time_parts = io::split(idle_str, ':');
			sum_time = stoi(time_parts[0]) * 60 + stoi(time_parts[1]);
			//std::cout << time_parts[0] << " min " << time_parts[1] << " sec " << std::endl;
		}

		// record for each user
		activity[tokens[0]] = sum_time;

		// find lowest value
		if (sum_time < new_idle_time) {
			new_idle_time = sum_time;
		}

	}
	return new_idle_time;
}

} // namespace sys
