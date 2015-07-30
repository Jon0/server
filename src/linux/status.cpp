#include <unistd.h>

#include "status.h"

namespace os {

status::status()
	:
	node("status"),
	pid("pid", getpid(), this),
	login("login", std::string(getlogin()), this) {
}

}
