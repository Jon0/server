#include <iostream>
#include <vector>

#include "../parser.h"
#include "power.h"

PowerCtrl::PowerCtrl() {
	idle_sec = 0;
	idle_shutdown_sec = 5 * 60;
}

void PowerCtrl::update() {
	auto w_out = cl.exec("w");
	auto lines = io::split(w_out, '\n');
	int new_idle_time = 1000000;

	// parse the lines from w
	for (int i = 2; i < lines.size(); ++i) {
		auto tokens = io::split(lines[i], ' ');
		auto idle_str = tokens[4];
		auto last_char = idle_str[idle_str.length() - 1];
		int sum_time = 1000000;
		if (last_char == 'm') {
			auto time_parts = io::split(idle_str.substr(0, idle_str.length() - 1), ':');
			sum_time = stoi(time_parts[0]) * 60 + stoi(time_parts[1]);
			//std::cout << time_parts[0] << " min " << time_parts[1] << " sec " << std::to_string(sum_time) << std::endl;
		}
		else if (last_char == 's') {
			auto time_parts = io::split(idle_str.substr(0, idle_str.length() - 1), '.');
			sum_time = stoi(time_parts[0]);
			//std::cout << time_parts[0] << " sec " << std::to_string(sum_time) << std::endl;
		}

		// find lowest value
		if (sum_time < new_idle_time) {
			new_idle_time = sum_time;
		}
	}

	idle_sec = new_idle_time;
	if (idle_sec > idle_shutdown_sec) {
		std::cout << "system shuting down in 1 minute" << std::endl;
		shutdown();
	}
}

int PowerCtrl::idle_seconds() {
	return idle_sec;
}

int PowerCtrl::idle_shutdown_seconds() {
	return idle_shutdown_sec;
}

void PowerCtrl::shutdown() {
	cl.exec("shutdown -P +1");
}