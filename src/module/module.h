#pragma once

#include <string>
#include <unordered_map>

namespace mod {

using object = int;


/**
 * abstract class
 */
class module {
public:
	virtual void write(const object &req) = 0;

};


/**
 * object matching by regex
 */
class registry : public module {
public:
	void write(const object &req) override;

	void add(std::string, module *);

private:
	std::unordered_map<std::string, module *> mods;
};


};
