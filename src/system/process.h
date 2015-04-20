#ifndef PROCESS_H
#define PROCESS_H

namespace sys {

class SambaMonitor {
public:
	SambaMonitor();

	void update();

	bool active_sessions();

	std::string html();

private:
	bool active;

};

} // namespace sys

#endif
