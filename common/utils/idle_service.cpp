#include "utils/idle_service.h"

IdleService::IdleService() : heartbeat_freq(30) {
    log = Log::getInstance();
}

bool IdleService::init() {
    start();
    return true;
}

void IdleService::destroy() {
    stop();
}

bool IdleService::threadLoop() {
    static int nap_count = 0;

    boost::this_thread::sleep(boost::posix_time::minutes(1));
    if ((nap_count++ % heartbeat_freq) == 0) {
        log->info("heartbeat");
    }

    return true;
}
