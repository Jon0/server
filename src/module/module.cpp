#include "module.h"

namespace mod {

void registry::write(const object &rp) {
	for (auto &m : mods) {
		m.second->write(rp);
	}
}

void registry::add(std::string, module *m) {

}

}
