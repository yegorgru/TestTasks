#ifndef LOGSYSTEMUTILS_H
#define LOGSYSTEMUTILS_H

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

template<typename T>
LogMessage::LogMessage(const T& message) requires Printable<T> {
	mStream << message;
}

template <typename T>
LogMessage& LogMessage::operator<<(const T& message) requires Printable<T> {
	mStream << message;
	return *this;
}

} // namespace LogSystemUtils

#endif LOGSYSTEMUTILS_H