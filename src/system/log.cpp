#include <chrono>
#include <ctime>

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
}

LogFile::LogFile(std::string path)
	:
	log_stream(path) {
	*this << "Started at " << time_str();
}

LogFile::~LogFile() {}

std::ofstream &LogFile::stream() {
	return log_stream;
}

std::string LogFile::time_str() {
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now);
	return std::ctime(&now_c);
}

LogFile &log() {
	return *LogFile::instance;
}

} // namespace sys
