#ifndef PROJECT_IDLE_SERVICE_H
#define PROJECT_IDLE_SERVICE_H

#include "utils/log.h"
#include "skeleton/skeleton_service.h"

class IdleService : public SkeletonService {
public:
    IdleService();

    bool init();

    void destroy();

protected:
    bool threadLoop() override;

private:
    Log *log;

    int32_t  heartbeat_freq;
};

#endif //PROJECT_IDLE_SERVICE_H
