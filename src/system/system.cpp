#include <iostream>

#include "log.h"
#include "system.h"

namespace sys {

System *System::instance = nullptr;

void System::create() {
	if (instance) {
		return;
	}
	instance = new System();
}

System *System::get() {
	return instance;
}

Config *System::get_config() {
	return &conf;
}

PowerCtrl *System::get_power_ctrl() {
	return &pwr;
}

SambaMonitor *System::get_samba() {
	return &smb;
}

MythTvMonitor *System::get_mythtv() {
	return &mtv;
}

System::System()
	:
	run(true) {
	logmsg("startup");
	dir = cl.exec("pwd");
}

System::~System() {
	run = false;
	update_thread.join();
}

void System::logmsg(const std::string &msg) const {
	std::cout << "[system] " << msg << "\n";
}

void System::update() {
	pwr.update();
	smb.update();
	mtv.update();
}

type::node System::make_node() const {
	int idle_time = pwr.idle_seconds() / 60;
	int shutdown_time = pwr.idle_shutdown_seconds() / 60;
	std::string idle_time_str = std::to_string(idle_time) + " minutes";
	std::string remain_str = std::to_string(shutdown_time - idle_time) + " minutes";

	type::node n("system");
	n.add("power", type::typed_value::create(pwr.html()));
	n.add("samba", type::typed_value::create(smb.html()));
	n.add("mythtv", type::typed_value::create(mtv.html()));
	n.add("idle_time", type::typed_value::create(idle_time_str));
	n.add("remain_time", type::typed_value::create(remain_str));
	n.add("log_path", type::typed_value::create(log().log_path()));
	n.add("run_path", type::typed_value::create(conf.filepath()));
	return n;
}

}
