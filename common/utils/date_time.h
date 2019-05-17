#ifndef PROJECT_TIME_H
#define PROJECT_TIME_H

#include <cstdint>
#include <string>
#include <ctime>
#include <memory>

enum class TimeFormat {
    YYYYMMDD_hhmmss,
};

enum class TimeZone {
    UTC08
};

class Time {
public:
    static time_t getTimestamp();

    static Time *getInstance();

    std::string getTimeString(TimeFormat time_format = TimeFormat::YYYYMMDD_hhmmss);

    static std::string timestamp_to_string(uint64_t ts, TimeZone tz = TimeZone::UTC08);

private:
    Time(TimeZone tz = TimeZone::UTC08);

    ~Time() = default;

    Time& operator = (Time &) = delete;

    Time(Time &) = delete;

    std::shared_ptr<::tm> getTm();

private:
    TimeZone time_zone_;
};


#endif //PROJECT_TIME_H
