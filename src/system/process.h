#pragma once

#include <vector>

#include "service.h"

namespace sys {

class SambaMonitor : public service {
public:
	SambaMonitor();

	bool available() const override;

	void update() override;

	int active_sessions();

	std::string html() const;

private:
	int active;
	std::vector<std::string> connections;

};


class MythTvMonitor : public service {
public:
	MythTvMonitor();

	bool available() const override;

	void update() override;

	int active_sessions();

	std::string html() const;

private:
	int active;
	std::vector<std::string> connections;

};

} // namespace sys
