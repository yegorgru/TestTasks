#ifndef LOGSYSTEMUTILS_H
#define LOGSYSTEMUTILS_H

#include <iostream>
#include <syncstream>


namespace LogSystem {

class Logger;

enum LogLevel {
	DEBUG,
	INFO,
	WARNING,
	ERROR
};

const char* logLevelToStr(LogLevel logLevel);

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
	
	LogMessage(const LogMessage&) = delete;
	LogMessage(LogMessage&&) = default;
	LogMessage& operator=(const LogMessage&) = delete;
	LogMessage& operator=(LogMessage&&) = default;
public:
	~LogMessage();
public:
	template <typename T>
	LogMessage& operator<<(const T& message) requires Printable<T>;
private:
	std::osyncstream mStream;
};


template<typename T>
LogMessage::LogMessage(const T& message) requires Printable<T>
	: mStream(std::cout)
{
	mStream << message;
}

template <typename T>
LogMessage& LogMessage::operator<<(const T& message) requires Printable<T> {
	mStream << message;
	return *this;
}

} // namespace LogSystem

#endif LOGSYSTEMUTILS_H