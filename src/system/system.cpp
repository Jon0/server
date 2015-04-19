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
	std::string idle_time = std::to_string(pwr.idle_seconds().count() / 60) + " minutes";
	std::string str;
	str += "<!DOCTYPE html>";
	str += "<html>";
	str += "<head>";
	str += "<title>test</title>";
	str += "</head>";
	str += "<body>";
	str += "<h1>system idle time "+idle_time+"</h1>";
	str += "<a href=\"/off\">turn off</a>";
	str += "</body>";
	str += "</html>";
	return str;
}

}
