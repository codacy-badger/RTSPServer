#ifndef PROJECT_RTSP_SERVICE_H
#define PROJECT_RTSP_SERVICE_H

#include "liveMedia.hh"
#include "ServerMediaSession.hh"
#include "BasicUsageEnvironment.hh"
#include "skeleton/skeleton_service.h"
#include "utils/stream_queue.h"
#include "server/rtsp_server_media_subsession.h"
#include "server/rtsp_parameter.h"
#include "server/rtsp_stream_server.h"

class RTSPService : public SkeletonService {
public:
    static std::shared_ptr<RTSPService> createNew(std::shared_ptr<RTSPParameter> param,
                                                  std::shared_ptr<StreamQueue> queue);

    bool init();

    void destroy();

    RTSPService(std::shared_ptr<RTSPParameter> param, std::shared_ptr<StreamQueue> queue);

    virtual ~RTSPService();

protected:
    virtual bool threadLoop();

private:
    void teardown();

    bool increaseSocketBufferMaxSize(uint64_t buf_size);

    bool writeValue(std::string fname, uint64_t value);

private:
    TaskScheduler *taskScheduler;

    UserAuthenticationDatabase *authDB;

    UsageEnvironment *env;

    RTSPStreamServer *streamServer;

    ServerMediaSession *serverMediaSession;

    RTSPServerMediaSubsession *serverMediaSubsession;

    Boolean reuseFirstSource_;

    uint32_t max_size_;

    std::shared_ptr<RTSPParameter> param_;

    std::shared_ptr<StreamQueue> streamQueue_;

    char volatile terminate_event_loop;
};

#endif //PROJECT_RTSP_SERVICE_H
