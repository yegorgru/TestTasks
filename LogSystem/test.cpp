#include "log.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <string>
#include <random>
#include <mutex>
#include <array>
#include <atomic>
#include <utility>
#include <limits>

class TestRunner {
public:
    TestRunner() 
        : mSharedCounter(0), mLogger("TestRunner")
    {
        mLogger << "Enter ctor";
        Logger logger{"CtorLogger"};

        logger(DEBUG) << "Running the threads";

        const int threadNumber = 8;
        mThreads.reserve(threadNumber);
        for (int i = 0; i < threadNumber; ++i) {
            mThreads.emplace_back(&TestRunner::run, this);
        }

        mLogger << "Leave ctor";
    }

    ~TestRunner() {
        mLogger << "Enter dtor";
        auto logger = getLogger("DtorLogger");
        logger(WARNING) << "Destructor. Threads will join";
        mLogger << "Leave dtor";
    }

    void run() {
        mLogger << "Enter run";
        Logger logger{ "RunLogger" };
        logger << "Starting run";

        std::random_device rd;
        std::mt19937 mt(rd());

        auto randomSleep = [&mt]() {
            auto sleepTime = mt() % 500;
            auto logger = getLogger("Sleep");
            logger(WARNING) << "Will sleep " << sleepTime << " ms";
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            logger(WARNING) << "Slept " << sleepTime << " ms";
        };

        auto randomYield = [&mt]() {
            int yieldTimes = mt() % 10;
            auto logger = getLogger("Yield");
            logger(DEBUG) << "Will yield " << yieldTimes << " times";
            for (int i = 0; i < yieldTimes; ++i) {
                std::this_thread::yield();
            }
            logger(DEBUG) << "Yielded " << yieldTimes << " times";
        };

        auto randomWait = [&]() {
            auto enterLogger = getLogger("WaitEnterLogger");
            enterLogger(WARNING) << "Entered wait";

            size_t mutexNumber = mt() % mMutexes.size();
            {
                std::lock_guard<std::mutex> lg(mMutexes[mutexNumber]);
                randomSleep();
                auto logger = getLogger("Wait");
                logger(ERROR) << "Counter: " << ++mSharedCounter;
                logger(DEBUG) << "Changed counter: " << mSharedCounter;
                randomSleep();
            }

            enterLogger << "Leaving wait";
        };

        const int iterations = 5;
        logger(DEBUG) << "Loop run start. " << iterations << " iterations...";
        for (int i = 0; i < iterations; ++i) {
            auto randValue = mt() % 5;
            if (randValue == 0) {
                logger(DEBUG) << "Going to sleep";
                randomSleep();
            }
            else if (randValue == 1) {
                logger(DEBUG) << "Going to yield";
                randomYield();
            }
            else if (randValue == 2) {
                logger(INFO) << "Going to wait";
                randomWait();
            }
            else {
                logger(ERROR) << "I'm lucky this time:)";
            }
            logger(DEBUG) << "Going to next iteration";
        }
        logger(DEBUG) << "Loop run end";

        logger << "Leaving run";

        mLogger << "Leave run";
    }

private:
    std::array<std::mutex, 3> mMutexes;
    std::atomic<int> mSharedCounter;
    Logger mLogger;
    std::vector<std::jthread> mThreads;
};

class LoggerOwner1 {
public:
    LoggerOwner1(Logger logger1, const Logger logger2, const Logger& logger3, Logger& logger4, Logger&& logger5)
        : mLogger(logger1)
        , mLoggerConst(logger2)
        , mLoggerConstRef(logger3)
        , mLoggerRef(logger4)
        , mLoggerMoved(std::move(logger5))
    {
    }

    void printMessage(const std::string& message) {
        mLogger << message;
        mLoggerConst << message;
        mLoggerConstRef << message;
        mLoggerRef << message;
        mLoggerMoved << message;
    }

    void printMessage(Logger logger, const std::string& message) {
        logger << message;
    }

    void printMessage(const Logger& logger, const std::string& message) {
        logger << message;
    }

    void printMessage(Logger& logger, const std::string& message) {
        logger << message;
    }

private:
    Logger mLogger;
    const Logger mLoggerConst;
    const Logger& mLoggerConstRef;
    Logger& mLoggerRef;
    Logger mLoggerMoved;
};

void creationTest() {
    {
        auto logger = getLogger("Logger1");
        logger << "1" << "2" << "3";
    }
    {
        Logger logger2("Logger2");
        logger2 << "4" << "5" << "6";
    }
    {
        Logger logger3{ "Logger3" };
        logger3 << "7" << "8" << "9";
    }

    {
        auto logger = getLogger(std::string("Logger1"));
        logger << "1" << "2" << "3";
    }
    {
        Logger logger(std::string("Logger2"));
        logger << "4" << "5" << "6";
    }
    {
        Logger logger{ std::string("Logger3") };
        logger << "7" << "8" << "9";
    }
    {
        const char* logName = "logger4";
        {
            auto logger = getLogger(logName);
            logger << "1" << "2" << "3";
        }
        {
            Logger logger(logName);
            logger << "4" << "5" << "6";
        }
        {
            Logger logger{ logName };
            logger << "7" << "8" << "9";
        }
    }
    {
        char logName[] = "logger5";
        {
            auto logger = getLogger(logName);
            logger << "1" << "2" << "3";
        }
        {
            Logger logger(logName);
            logger << "4" << "5" << "6";
        }
        {
            Logger logger{ logName };
            logger << "7" << "8" << "9";
        }
    }
    {
        std::string name = "Logger6";
        {
            auto logger = getLogger(name);
            logger << "1" << "2" << "3";
        }
        {
            Logger logger(name);
            logger << "4" << "5" << "6";
        }
        {
            Logger logger{ name };
            logger << "7" << "8" << "9";
        }
    }
    {
        const std::string name = "Logger7";
        {
            auto logger = getLogger(name);
            logger << "1" << "2" << "3";
        }
        {
            Logger logger(name);
            logger << "4" << "5" << "6";
        }
        {
            Logger logger{ name };
            logger << "7" << "8" << "9";
        }
    }
    {
        std::string name = "Logger8";
        {
            auto logger = getLogger(std::move(name));
            logger << "1" << "2" << "3";
        }
        name = "Logger9";
        {
            Logger logger(std::move(name));
            logger << "4" << "5" << "6";
        }
        name = "Logger10";
        {
            Logger logger{ std::move(name) };
            logger << "7" << "8" << "9";
        }
    }
    {
        auto logger = getLogger("Logger1");
        auto logger2 = logger;
        logger2 << "456";
        logger << "123";
    }
    {
        auto logger = getLogger("Logger1");
        auto logger2(logger);
        logger2 << "456";
        logger << "123";
    }
    {
        auto logger = getLogger("Logger1");
        auto logger2 = std::move(logger);
        logger2 << "456";
        //logger << "123";
    }
    {
        auto logger = getLogger("Logger1");
        auto logger2(std::move(logger));
        logger2 << "456";
        //logger << "123";
    }
    {
        const auto logger = getLogger("LoggerConst");
        logger << "456";
    }
    {
        auto logger = getLogger("LoggerRef");
        auto& loggerRef = logger;
        loggerRef << "456";
        logger << "123";
    }
    {
        const auto logger = getLogger("LoggerConstRef");
        const auto& loggerRef = logger;
        loggerRef << "456";
        logger << "123";
    }
    {
        auto logger = std::make_shared<Logger>("LoggerShared");
        *logger << "123";
        auto logger2 = logger;
        *logger2 << "456";
        *logger << "789";
        auto logger3(std::move(logger));
        *logger3 << "789";
        *logger2 << "456";
        std::weak_ptr<Logger> weak = logger3;
        *weak.lock() << "10";
    }
    {
        auto logger = std::make_unique<Logger>("LoggerUnique");
        *logger << "123";
        auto logger3(std::move(logger));
        *logger3 << "789";
    }

    Logger l1{ "1" };
    Logger l2{ "2" };
    Logger l3{ "3" };
    Logger l4{ "4" };
    Logger l5{ "5" };
    LoggerOwner1 lo(l1, l2, l3, l4, std::move(l5));

    lo.printMessage("m1");
}

struct PrintableStruct {
    int mA = 5;
    int mB = 6;
};

std::ostream& operator<<(std::ostream& os, const PrintableStruct& ab) {
    os << ab.mA << "|" << ab.mB;
    return os;
}

void testTypes() {
    Logger logger{ "abc" };
    const std::string str = "QQQ";
    std::string_view sv = "WWW";
    float f = 5.3;
    PrintableStruct ab;
    PrintableStruct cd{ 6, 7 };
    logger << "abc" << ' ' << std::string("bcd") << ' ' << str << ' ' << sv << ' ' << 1 << ' ' << 2u << ' ' << f << ' ' << 6.7 << ' ' << std::numeric_limits<unsigned long long>::max() << ' ' << std::numeric_limits<long long>::min() << ' ' << 'c' << ' ' << false << true << std::boolalpha << false << true << " " << ab << " " << cd;
}

int main() {
    //auto logger = getLogger("Main");
    //logger << "Starting the app";

    //{
    //    logger << "Creating test object";
    //    TestRunner testRunner;
    //    logger << "Leaving test object scope";
    //}

    //logger(DEBUG) << "Threads joined";

    //logger << "Finishing the app";

    creationTest();
    testTypes();

    return 0;
}