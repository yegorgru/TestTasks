#ifndef LOG_H
#define LOG_H

#include "LogSystemUtils.h"

#include <string>

namespace LogSystem {

class Logger {
public:
	Logger(const std::string& name);
public:
	LogMessage operator()(LogLevel logLevel);
	template <typename T>
	LogMessage operator<<(const T& message);
};

Logger getLogger(const std::string& name = "");


template <typename T>
LogMessage Logger::operator<<(const T& message) {
	// check if emit will be called
	LogMessage logMessage(logLevelToStr(INFO));
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