#include "utils/log.h"

Log *Log::getInstance() {
    static Log log;
    return &log;
}

void Log::init() {
    try {
        std::vector<spdlog::sink_ptr> sinks_;
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::info);

        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("./logs.txt", 1024*1024*10, 1);
        file_sink->set_level(spdlog::level::debug);

        sinks_.push_back(console_sink);
        sinks_.push_back(file_sink);

        log_handle = std::make_shared<spdlog::logger>("ourLog", begin(sinks_), end(sinks_));
        log_handle->set_pattern("[%D %H:%M:%S.%e] [%l] %v");

#ifndef NDEBUG
        setVerbosity(spdlog::level::level_enum::debug);
#else
        setVerbosity(spdlog::level::level_enum::info);
#endif

        spdlog::register_logger(log_handle);
    }
    catch (const spdlog::spdlog_ex &ex) {
        printf("Log init failed");
    }
}

void Log::destroy() {
    spdlog::drop_all();
}

void Log::setVerbosity(spdlog::level::level_enum log_level) {
    log_handle->set_level(log_level);
}