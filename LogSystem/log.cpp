#include "log.h"

Logger::Logger(const std::string& name) {

}

Logger& Logger::operator()(LogLevel logLevel) {
	return *this;
}

Logger getLogger(const std::string& name) {
	return Logger(name);
}
