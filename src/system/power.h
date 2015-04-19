#ifndef POWER_H
#define POWER_H

#include <chrono>
#include <string>

#include "command.h"

class PowerCtrl {
public:
	PowerCtrl();

	void update();

	int idle_seconds();
	int idle_shutdown_seconds();

	void shutdown();

private:
	int idle_shutdown_sec;
	int idle_sec;

	CommandLine cl;

};

#endif