#include "log.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <string>
#include <random>
#include <mutex>

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
    }

    ~TestRunner() {
        mLogger << "Enter dtor";
        auto logger = getLogger("DtorLogger");
        logger(WARNING) << "Destructor. Threads will join";
    }

    void run() {
        mLogger << "Enter run";
        Logger logger{ "RunLogger" };
        logger << "Starting run";

        std::random_device rd;
        std::mt19937 mt(rd());

        auto randomSleep = [&mt]() {
            auto sleepTime = mt() % 3000;
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

            {
                std::lock_guard<std::mutex> lg(mMutex);
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
    }

private:
    std::mutex mMutex;
    int mSharedCounter;
    Logger mLogger;
    std::vector<std::jthread> mThreads;
};

int main() {
    auto logger = getLogger("Main");
    logger << "Starting the app";

    {
        logger << "Creating test object";
        TestRunner testRunner;
        logger << "Leaving test object scope";
    }

    logger(DEBUG) << "Threads joined";

    logger << "Finishing the app";

    return 0;
}