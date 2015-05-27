#include <chrono>
#include <ctime>
#include <thread>

#include "log.h"

namespace sys {

LogFile *LogFile::instance = nullptr;

void LogFile::init(std::string path) {
	if (instance) {
		return;
	}
	instance = new LogFile(path);
}

void LogFile::close() {
	delete instance;
	instance = nullptr;
}

LogFile::LogFile(std::string path)
	:
	logpath(path + "_" + time_str()),
	log_stream(logpath) {

	// try reopen
	while (!log_stream.is_open()) {
		std::this_thread::sleep_for(std::chrono::seconds(10));
		log_stream.open(logpath , std::ios_base::out);
	}
	*this << "Started at " << time_str();
}

LogFile::~LogFile() {}

std::ofstream &LogFile::stream() {
	return log_stream;
}

std::string LogFile::time_str() {
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now);
	char buff [100];
	strftime (buff, 100, "%Y_%m_%d_%H_%M_%S", localtime(&now_c));
	return std::string(buff);
}

std::string LogFile::log_path() const {
	if (!log_stream.is_open()) {
		return logpath + " (failed open)";
	}
	return logpath;
}

LogFile &log() {
	return *LogFile::instance;
}

} // namespace sys
