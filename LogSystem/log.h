#ifndef LOG_H
#define LOG_H

#include "LogSystemUtils.h"

#include <string>

class Logger {
public:
	Logger(const std::string& name);
public:
	LogSystemUtils::LogMessage operator()(LogLevel logLevel);
	template <typename T>
	LogSystemUtils::LogMessage operator<<(const T& message);
};

Logger getLogger(const std::string& name = "");


template <typename T>
LogSystemUtils::LogMessage Logger::operator<<(const T& message) {
	return LogSystemUtils::LogMessage(message);
}

#endif	// LOG_H