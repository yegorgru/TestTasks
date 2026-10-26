#include "log.h"

namespace LogSystem {

Logger::Logger(const std::string& name)
	: mName(name)
{ }

LogMessage Logger::operator()(LogLevel logLevel) {
	return LogMessage(logLevel, mName);
}

Logger getLogger(const std::string& name) {
	return Logger(name);
}

}