#include <iostream>
#include <dirent.h>

#include "file.h"

namespace os {

directory::directory(const std::string &path)
	:
	node("directory"),
	path("path", path, this),
	files("files", file_list(), this) {
}

directory::~directory() {}

std::vector<std::string> directory::file_list() {
	std::vector<std::string> names;

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path.get().c_str())) != NULL) {

		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			std::string filename = ent->d_name;
			if (!filename.empty()) {
				names.push_back(filename);
			}
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
	}

	return names;
}

}
