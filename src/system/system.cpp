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

PowerCtrl *System::get_power_ctrl() {
	return &pwr;
}

SambaMonitor *System::get_samba() {
	return &smb;
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
	str += "<title>test</title>";
	str += "</head>";
	str += "<body>";
	str += "<h1>system idle time "+idle_time_str+"</h1>";
	str += "<h1>shutdown in "+remain_str+"</h1>";
	str += "<a href=\"/off\">turn off now</a>";
	str += "<br>";
	str += pwr.html();
	str += "<br>";
	str += smb.html();
	str += "</body>";
	str += "</html>";
	return str;
}

System::System()
	:
	run(true) {

	// start update thread
	update_thread = std::thread([this] {
		while (run) {
			pwr.update();
			smb.update();
			std::this_thread::sleep_for(std::chrono::seconds(10));
		}
	});
}

System::~System() {
	run = false;
	update_thread.join();
}

}
