#ifndef POWER_H
#define POWER_H

#include <chrono>
#include <string>

#include "command.h"

class PowerCtrl {
public:
	PowerCtrl();

	std::chrono::duration<double> idle_seconds();

	void shutdown();

private:

	CommandLine cl;

	// start time
	std::chrono::time_point<std::chrono::system_clock> start;

};

#endif