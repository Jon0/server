#include "power.h"

PowerCtrl::PowerCtrl() {
	start = std::chrono::system_clock::now();
}

std::chrono::duration<double> PowerCtrl::idle_seconds() {
	auto end = std::chrono::system_clock::now();
	return end - start;
}

void PowerCtrl::shutdown() {
	cl.exec("shutdown -P +1");
}