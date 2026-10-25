#include <string>
#include <iostream>

enum LogLevel {
	TRACE,
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	CRITICAL
};

class Logger {
public:
	Logger(const std::string& name);
public:
	//maybe use as return type some another object that will have stream inside
	Logger& operator()(LogLevel logLevel);
	template <typename T>
	Logger& operator<<(const T& message);
};

template <typename T>
Logger& Logger::operator<<(const T& message) {
	std::cout << message << std::endl;
	return *this;
}

Logger getLogger(const std::string& name = "");