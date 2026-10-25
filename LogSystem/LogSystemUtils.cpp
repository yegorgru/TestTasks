#include "LogSystemUtils.h"


namespace LogSystemUtils {

LogMessage::~LogMessage() {
	std::cout << mStream.str() << std::endl;
}

} // namespace LogSystemUtils