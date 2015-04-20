#ifndef SYSTEM_H
#define SYSTEM_H

#include <thread>

#include "power.h"
#include "process.h"

namespace sys {

class System {
public:

	static void create();
	static System *get();

	PowerCtrl *get_power_ctrl();
	SambaMonitor *get_samba();

	/**
	 * generate an html status page
	 */
	std::string html();

private:
	System();
	~System();

	bool run;
	std::thread update_thread;

	PowerCtrl pwr;
	SambaMonitor smb;

	static System *instance;
};

} // namespace system

#endif