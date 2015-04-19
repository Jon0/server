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

std::string System::html() {
	int idle_time = (int)(pwr.idle_seconds().count() / 60);
	std::string idle_time_str = std::to_string(idle_time) + " minutes";
	std::string remain_str = std::to_string(120 - idle_time) + " minutes";
	std::string str;
	str += "<!DOCTYPE html>";
	str += "<html>";
	str += "<head>";
	str += "<title>test</title>";
	str += "</head>";
	str += "<body>";
	str += "<h1>system idle time "+idle_time_str+"</h1>";
	str += "<h1>shutdown in "+remain_str+"</h1>";
	str += "<a href=\"/off\">turn off</a>";
	str += "</body>";
	str += "</html>";
	return str;
}

}
