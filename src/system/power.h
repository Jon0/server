#ifndef POWER_H
#define POWER_H

#include <chrono>
#include <string>
#include <unordered_map>

#include "../linux/command.h"
#include "service.h"

namespace sys {

class PowerCtrl : public service {
public:
	PowerCtrl();

	bool available() const override;

	void update() override;

	/**
	 * seconds all users have been idle for
	 */
	int idle_seconds() const;
	int idle_shutdown_seconds() const;


	/**
	 * change idle seconds required for shutdown
	 */
	void set_idle_seconds(int s);

	std::string html() const;

	/**
	 * turns off the machine
	 */
	void shutdown();

private:
	std::chrono::time_point<std::chrono::system_clock> last_update;
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
