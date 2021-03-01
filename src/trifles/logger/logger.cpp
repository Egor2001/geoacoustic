#include "logger.hpp"

#include <ctime>
#include <cstdint>
#include <cassert>

#include <string_view>
#include <chrono>
#include <ostream>

#include "macro.hpp"

/* 
 * @brief simple logger class
 * @author geome_try
 * @date 2020
 */

namespace trifles {

void TfLogger::log(std::string_view what, Level level)
{
    time_t now = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now());

    Entry entry = {
        .what = what,
        .when = now,
        .level = level
    };

    entries_.push(std::move(entry));
}

void TfLogger::flush(std::ostream& stream)
{
    while (!entries_.empty())
    {
        Entry entry = entries_.pop();
        std::string_view strlevel;

        switch (entry.level)
        {
            case Level::ERROR:
                strlevel = "ERROR";
                break;

            case Level::DEBUG:
                strlevel = "DEBUG";
                break;

            default:
                TRIFLES_ASSERT("impossible");
        }

        stream << std::put_time(std::gmtime(&entry.when), "[%H:%M:%S] ");
        stream << "[" << strlevel << "] " << what << "\n";
    }
}

} // namespace trifles 

#endif // TRIFLES_LOGGER_LOGGER_HPP
