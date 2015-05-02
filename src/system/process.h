#ifndef PROCESS_H
#define PROCESS_H

#include <vector>

namespace sys {

class SambaMonitor {
public:
	SambaMonitor();

	void update();

	int active_sessions();

	std::string html();

private:
	int active;
	std::vector<std::string> connections;

};

} // namespace sys

#endif
