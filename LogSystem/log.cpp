#include "log.h"

namespace LogSystem {

Logger::Logger(const std::string& name) {

}

LogMessage Logger::operator()(LogLevel logLevel) {
	return LogMessage(logLevel);
}

Logger getLogger(const std::string& name) {
	return Logger(name);
}

}