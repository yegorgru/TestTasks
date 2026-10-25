#include "log.h"

namespace {

const char* logLevelToStr(LogLevel logLevel) {
	switch (logLevel) {
	case DEBUG:
		return "DEBUG";
	case WARNING:
		return "WARNING";
	case ERROR:
		return "ERROR";
	default:
		return "INFO";
	}
}

}

namespace LogSystemUtils {

LogMessage::~LogMessage() {
	std::cout << mStream.str() << std::endl;
}

} // namespace LogSystemUtils

using namespace LogSystemUtils;

Logger::Logger(const std::string& name) {

}

LogMessage Logger::operator()(LogLevel logLevel) {
	return LogMessage(logLevelToStr(logLevel));
}

Logger getLogger(const std::string& name) {
	return Logger(name);
}
