#ifndef LOG_H
#define LOG_H

#include "LogSystemUtils.h"

#include <string>

namespace LogSystem {

// potential improvements: write file/funtion name and line:
// std::source_location::current()

class Logger {
public:
	Logger(const std::string& name);

	Logger(const Logger&) = default;
	Logger(Logger&&) = default;
	Logger& operator=(const Logger&) = default;
	Logger& operator=(Logger&&) = default;
public:
	LogMessage operator()(LogLevel logLevel) const;
	template <typename T>
	LogMessage operator<<(const T& message) const;
private:
	std::string mName;
};

Logger getLogger(const std::string& name = "");


template <typename T>
LogMessage Logger::operator<<(const T& message) const {
	LogMessage logMessage(INFO, mName);
	logMessage << message;
	return logMessage;
}

} // namespace LogSystem

using Logger = LogSystem::Logger;
using LogSystem::getLogger;
using LogSystem::LogLevel::DEBUG;
using LogSystem::LogLevel::INFO;
using LogSystem::LogLevel::WARNING;
using LogSystem::LogLevel::ERROR;

#endif	// LOG_H