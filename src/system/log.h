#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <string>

namespace sys {

class LogFile {
public:
	virtual ~LogFile();

	/**
	 * stream for writing
	 */
	std::ofstream &stream();

	std::string time_str();

	std::string log_path() const;

	template<class T>
	LogFile &operator <<(const T &t) {
		stream() << t;
		stream().flush();
		return *this;
	}

	static LogFile *instance;
	static void init(std::string path);
	static void close();

private:
	LogFile(std::string path);
	std::string logpath;
	std::ofstream log_stream;

};

LogFile &log();

} // namespace sys

#endif