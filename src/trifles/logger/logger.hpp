#ifndef TRIFLES_LOGGER_LOGGER_HPP
#define TRIFLES_LOGGER_LOGGER_HPP

#include <ctime>
#include <cstdint>

#include <string>
#include <string_view>
#include <queue>
#include <ostream>

namespace trifles {

class TfLogger 
{
public:
    enum class Level : uint16_t
    {
        ERROR = 40000, DEBUG = 10000
    };

    struct Entry
    {
        std::string what;
        time_t when;
        Level level;
    };

    void log(std::string_view what, Level level);
    void flush(std::ostream& stream);

protected:
    TfLogger() = default;
    virtual ~TfLogger() = default;

private:
    std::queue<Entry> entries_;
};

} // namespace trifles 

#endif // TRIFLES_LOGGER_LOGGER_HPP
