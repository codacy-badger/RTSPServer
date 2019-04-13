//
// Created by kang on 2018/12/14.
//

#include <chrono>  //NOLINT
#include "utils/date_time.h"
#include "skeleton/exception.h"

using namespace std::chrono;

Time::Time(TimeZone tz) : time_zone_(tz) {
}

Time *Time::getInstance() {
    static Time time(TimeZone::UTC08);
    return &time;
}

time_t Time::getTimestamp() {
    return system_clock::to_time_t(system_clock::now());
}

std::string Time::getTimeString(TimeFormat time_format) {
    std::string time_str;
    auto utc_tm = getTm();
    char buf[64];

    switch (time_format) {
        case TimeFormat::YYYYMMDD_hhmmss : {
            snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d", utc_tm->tm_year + 1900, utc_tm->tm_mon + 1,
                     utc_tm->tm_mday, utc_tm->tm_hour, utc_tm->tm_min, utc_tm->tm_sec);

            time_str = std::string(buf);
            break;
        }

        default: {
            exception::runtimeAssert("Not support such time format!", exception::ErrorCode::invalidParameter);
        }
    }
    return time_str;
}

std::string Time::timestamp_to_string(uint64_t ts, TimeZone tz) {
    struct tm utc_tm;
    time_t tmp_ts = ts;
    char buf[128] = "";
    switch (tz) {
        case TimeZone::UTC08 : {
            tmp_ts += 8*60*60;
            break;
        }

        default: {
            exception::runtimeAssert("Not support such time zone!", exception::ErrorCode::invalidParameter);
        }
    }
    if (nullptr != gmtime_r(&tmp_ts, &utc_tm)) {
        snprintf(buf,
                 sizeof(buf),
                 "%04d-%02d-%02d_%02d%02d%02d",
                 utc_tm.tm_year + 1900,
                 utc_tm.tm_mon + 1,
                 utc_tm.tm_mday,
                 utc_tm.tm_hour,
                 utc_tm.tm_min,
                 utc_tm.tm_sec);
        return std::string(buf);
    }
    return std::string("");
}

std::shared_ptr<::tm> Time::getTm() {
    ::tm utc_tm;
    auto ts = getTimestamp();

    switch (time_zone_) {
        case TimeZone::UTC08 : {
            ts += 8*60*60;
            break;
        }

        default: {
            exception::runtimeAssert("Not support such time zone!", exception::ErrorCode::invalidParameter);
        }
    }

    if (nullptr != gmtime_r(&ts, &utc_tm)) {
        std::shared_ptr<tm> tm_ptr = std::make_shared<tm>();
        *tm_ptr = utc_tm;
        return tm_ptr;
    }
    return nullptr;
}
