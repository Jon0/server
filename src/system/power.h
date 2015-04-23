#ifndef POWER_H
#define POWER_H

#include <chrono>
#include <string>
#include <unordered_map>

#include "command.h"

namespace sys {

class PowerCtrl {
public:
	PowerCtrl();

	void update();

	/**
	 * seconds all users have been idle for
	 */
	int idle_seconds();
	int idle_shutdown_seconds();


	/**
	 * change idle seconds required for shutdown
	 */
	void set_idle_seconds(int s);

	std::string html();

	/**
	 * turns off the machine
	 */
	void shutdown();

private:
	std::chrono::time_point<std::chrono::system_clock> start;
	int idle_sec;

	std::unordered_map<std::string, int> activity;

	CommandLine cl;

	/**
	 * parse output of system w command
	 */
	int w_idle_sec();

};

} // namespace sys

#endif
