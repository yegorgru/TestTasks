#include "LogSystemUtils.h"

namespace LogSystem {

LogMessage::~LogMessage() {
	mStream << std::endl;
}

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

} // namespace LogSystem

