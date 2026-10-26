#include "LogSystemUtils.h"

#include <chrono>
#include <format>

namespace LogSystem {

LogMessage::LogMessage(const LogLevel logLevel, const std::string& name)
	: mStream(std::cout)
{
	printDateTime();
	mStream << "; ";
	printLogLevel(logLevel);
	mStream << "; " << name << "(" << std::this_thread::get_id() << "): ";
}

LogMessage::~LogMessage() {
	mStream << std::endl;
}

void LogMessage::printDateTime() {
	auto time = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()) };
	mStream << std::format("{0:%d.%m.%Y %H:%M:%S}", time);
}

void LogMessage::printLogLevel(const LogLevel logLevel) {
	switch (logLevel) {
	case DEBUG:
		mStream << "DEBUG";
		return;
	case WARNING:
		mStream << "WARNING";
		return;
	case ERROR:
		mStream << "ERROR";
		return;
	default:
		mStream << "INFO";
		return;
	}
}

} // namespace LogSystem

