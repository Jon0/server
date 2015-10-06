#ifndef SYSTEM_H
#define SYSTEM_H

#include <thread>

#include "../type/var.h"
#include "config.h"
#include "power.h"
#include "process.h"

namespace sys {

class System {
public:

	static void create();
	static System *get();


	CommandLine cl;
	Config *get_config();
	PowerCtrl *get_power_ctrl();
	SambaMonitor *get_samba();
	MythTvMonitor *get_mythtv();

	type::node make_node() const;

private:
	System();
	~System();

	void logmsg(const std::string &msg) const;

	void update();

	// initialise config
	Config conf;

	PowerCtrl pwr;
	SambaMonitor smb;
	MythTvMonitor mtv;

	std::string dir;

	// update thread control
	bool run;
	std::thread update_thread;

	static System *instance;
};

} // namespace system

#endif