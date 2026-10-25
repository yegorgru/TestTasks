#include <string>
#include <sstream>
#include <iostream>


enum LogLevel {
	DEBUG,
	INFO,
	WARNING,
	ERROR
};

class Logger;


namespace LogSystemUtils {

template<typename T>
concept Printable = requires (T a)
{
	std::cout << a;
};


class LogMessage {
	friend class Logger;
private:
	template<typename T>
	LogMessage(const T& message) requires Printable<T>;
public:
	~LogMessage();
public:
	template <typename T>
	LogMessage& operator<<(const T& message) requires Printable<T>;
private:
	std::ostringstream mStream;
};

} // namespace LogSystemUtils


class Logger {
public:
	Logger(const std::string& name);
public:
	LogSystemUtils::LogMessage operator()(LogLevel logLevel);
	template <typename T>
	LogSystemUtils::LogMessage operator<<(const T& message);
};

Logger getLogger(const std::string& name = "");



namespace LogSystemUtils {

template<typename T>
LogMessage::LogMessage(const T& message) requires Printable<T> {
	mStream << message;
}

template <typename T>
LogMessage& LogMessage::operator<<(const T& message) requires Printable<T> {
	mStream << message;
	return *this;
}

}



template <typename T>
LogSystemUtils::LogMessage Logger::operator<<(const T& message) {
	return LogSystemUtils::LogMessage(message);
}