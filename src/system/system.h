#ifndef SYSTEM_H
#define SYSTEM_H

#include "power.h"

namespace sys {

class System {
public:
	
	static void create();
	static System *get();

	PowerCtrl *get_power_ctrl();
	std::string html();

private:
	System() {}

	PowerCtrl pwr;

	static System *instance;
};

} // namespace system

#endif