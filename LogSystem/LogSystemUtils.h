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

template<typename T>
concept Printable = requires (T a)
{
	std::cout << a;
};

class LogMessage {
	friend class Logger;
private:
	LogMessage(const LogLevel logLevel, const std::string& name);
	
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
	void printDateTime();
	void printLogLevel(const LogLevel logLevel);
private:
	std::osyncstream mStream;
};


template <typename T>
LogMessage& LogMessage::operator<<(const T& message) requires Printable<T> {
	mStream << message;
	return *this;
}

} // namespace LogSystem

#endif LOGSYSTEMUTILS_H