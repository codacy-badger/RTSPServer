#ifndef PROJECT_LOG_H
#define PROJECT_LOG_H

#include <string>
#include <sstream>
#include <fmt/printf.h>
#include <fmt/format.h>
#include <spdlog/logger.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

class Log {
public:
    static Log *getInstance();

    void init();

    void destroy();

    void setVerbosity(spdlog::level::level_enum log_level);

    template<typename... Args>
    void tracef(const char *arg, const Args &... args) {
        log_handle->trace(fmt::sprintf(arg, args ...));
    }

    template<typename... Args>
    void trace(const char *arg, const Args &... args) {
        log_handle->trace(arg, args ...);
    }

    template<typename... Args>
    void debugf(const char *arg, const Args &... args) {
        log_handle->debug(fmt::sprintf(arg, args ...));
    }

    template<typename... Args>
    void debug(const char *arg, const Args &... args) {
        log_handle->debug(arg, args ...);
    }

    template<typename... Args>
    void infof(const char *arg, const Args &... args) {
        log_handle->info(fmt::sprintf(arg, args ...));
    }

    template<typename... Args>
    void info(const char *arg, const Args &... args) {
        log_handle->info(arg, args ...);
    }

    template<typename... Args>
    void warningf(const char *arg, const Args &... args) {
        log_handle->warn(fmt::sprintf(arg, args ...));
    }

    template<typename... Args>
    void warning(const char *arg, const Args &... args) {
        log_handle->warn(arg, args ...);
    }

    template<typename... Args>
    void errorf(const char *arg, const Args &... args) {
        log_handle->error(fmt::sprintf(arg, args ...));
    }

    template<typename... Args>
    void error(const char *arg, const Args &... args) {
        log_handle->error(arg, args ...);
    }

    template<typename... Args>
    void criticalf(const char *arg, const Args &... args) {
        log_handle->critical(fmt::sprintf(arg, args ...));
    }

    template<typename... Args>
    void critical(const char *arg, const Args &... args) {
        log_handle->critical(arg, args ...);
    }

private:
    Log() = default;

    ~Log() = default;

    Log(Log const &) = delete;

    Log &operator=(Log const &) = delete;

    std::shared_ptr<spdlog::logger> log_handle;
};

#endif //PROJECT_LOG_H



