#pragma once

namespace sys {

class service {
public:
	virtual ~service() {}

	virtual bool available() const = 0;

	virtual void update() = 0;
};

} // sys
