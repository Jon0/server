#ifndef SYSTEM_H
#define SYSTEM_H

#include <thread>

#include "config.h"
#include "power.h"
#include "process.h"

namespace sys {

class System {
public:

	static void create();
	static System *get();

	Config *get_config();
	PowerCtrl *get_power_ctrl();
	SambaMonitor *get_samba();

	/**
	 * generate an html status page
	 */
	std::string html();

private:
	System();
	~System();

	// initialise config
	Config conf;

	PowerCtrl pwr;
	SambaMonitor smb;

	// update thread control
	bool run;
	std::thread update_thread;

	static System *instance;
};

} // namespace system

#endif