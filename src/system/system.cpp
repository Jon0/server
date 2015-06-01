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

std::string System::html() {
	int idle_time = pwr.idle_seconds() / 60;
	int shutdown_time = pwr.idle_shutdown_seconds() / 60;
	std::string idle_time_str = std::to_string(idle_time) + " minutes";
	std::string remain_str = std::to_string(shutdown_time - idle_time) + " minutes";
	std::string str;
	str += "<!DOCTYPE html>";
	str += "<html>";
	str += "<head>";
	str += "<title>power</title>";
	str += "</head>";
	str += "<body>";
	str += "<h1>system idle time "+idle_time_str+"</h1>";
	str += "<h1>shutdown in "+remain_str+"</h1>";
	str += "<form action=\"off\">";
	str += "<input type=\"submit\" value=\"Turn off now\">";
	str += "</form>";
	str += "<hr>";
	str += pwr.html();
	str += "<hr>";
	str += smb.html();
	str += "<hr>";
	str += mtv.html();
	str += "<hr>";
	str += "<h3>"+log().log_path()+"</h3>";
	str += "<h3>"+get_config()->filepath()+"</h3>";
	str += "</body>";
	str += "</html>";
	return str;
}

System::System()
	:
	run(true) {

	CommandLine cl;
	dir = cl.exec("pwd");

	// start update thread
	update_thread = std::thread([this] {
		while (run) {
			std::this_thread::sleep_for(std::chrono::seconds(10));
			pwr.update();
			smb.update();
			mtv.update();
		}
	});
}

System::~System() {
	run = false;
	update_thread.join();
}

}
