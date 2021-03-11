#include "RC.h"
#include "config/Config.h"
#include "hardware/IO.h"
#include "scheduler/Scheduler.h"
#include "rc/drivers/AllDrivers.h"
#include "log/Log.h"

namespace RC {
    namespace { // private
        const auto LogTag = "RC";

        Driver* driver = nullptr;
        uint32_t lastFrameTimeMs = 0;

        bool shouldTryToGetFrame() {
            // driver guaranteed not to be null because otherwise the task would never have been scheduled.
            return driver->shouldTryToGetFrame();
        }

        const Scheduler::Name RCTaskName = "RC Receiver";
        void tryGetFrame() {
            // driver guaranteed not to be null because otherwise the task would never have been scheduled.
            if (driver->getFrame(channels)) {
                lastFrameTimeMs = millis();
            }
        }
    }

    Channels channels;

    void initialize() {
        // Choose driver
        if (Config::config.has_rcConfig) {
            switch (Config::config.rcConfig.which_driverConfig) {
                case RCConfig_crossfire_tag: {
                    auto uart = IO::takeUart(Config::config.rcConfig.driverConfig.ibus.uartIndex);
                    if (uart != nullptr) {
                        driver = new RcDrivers::CrossfireDriver(*uart);
                    } else {
                        Log::error(LogTag, "RC Driver UART not valid.");
                    }
                    break;
                }
                case RCConfig_ibus_tag: {
                    auto uart = IO::takeUart(Config::config.rcConfig.driverConfig.ibus.uartIndex);
                    if (uart != nullptr) {
                        driver = new RcDrivers::IBUSDriver(*uart);
                    } else {
                        Log::error(LogTag, "RC Driver UART not valid.");
                    }
                    break;
                }
                default:
                    Log::error(LogTag, "RC Driver Config type invalid.");
            }
        } else {
            Log::warning(LogTag, "No RC Driver configured.");
        }

        if (driver != nullptr) {
            // Initialize channels
            for (size_t i = 0; i < min(driver->channelCount, MaxChannelCount); i++) {
                channels.push_back(0);
            }

            // Setup get RC data task
            auto task = new Scheduler::Task(tryGetFrame, RCTaskName);
            auto runner = new Scheduler::AdHocTaskRunner(task, shouldTryToGetFrame);
            Scheduler::addTaskRunner(runner);

            Log::info(LogTag, "RC Driver configured.");
        }
    }

    uint32_t timeSinceLastFrame() {
        return millis() - lastFrameTimeMs;
    }
}
