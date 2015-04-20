#ifndef PROCESS_H
#define PROCESS_H

namespace sys {

class SambaMonitor {
public:
	SambaMonitor();

	void update();

	int active_sessions();

	std::string html();

private:
	int active;

};

} // namespace sys

#endif
