#include "skeleton/exception.h"
#include "server/rtsp_stream_input.h"
#include "server/rtsp_service.h"

using namespace exception;

std::shared_ptr<RTSPService> RTSPService::createNew(std::shared_ptr<RTSPParameter> param,
                                                    std::shared_ptr<StreamQueue> queue) {
    return std::make_shared<RTSPService>(param, queue);
}

RTSPService::RTSPService(std::shared_ptr<RTSPParameter> param, std::shared_ptr<StreamQueue> queue)
    : param_(param),
      streamQueue_(queue),
      max_size_(2 << 20),
      reuseFirstSource_(true),
      taskScheduler(nullptr),
      authDB(nullptr),
      env(nullptr),
      streamServer(nullptr),
      serverMediaSession(nullptr),
      serverMediaSubsession(nullptr),
      terminate_event_loop(false) {
    log = Log::getInstance();
    runtimeAssert("invalid RTSP parameter", (nullptr != param_ && nullptr != streamQueue_));

    increaseSocketBufferMaxSize(4096*1024);

    taskScheduler = BasicTaskScheduler::createNew();
    runtimeAssert("BasicTaskScheduler::createNew failed", nullptr != taskScheduler);

    env = BasicUsageEnvironment::createNew(*taskScheduler);
    runtimeAssert("BasicUsageEnvironment::createNew failed", nullptr != env);

    authDB = new UserAuthenticationDatabase;
    for (auto auth_iterator = param_->account.begin(); auth_iterator != param_->account.end(); auth_iterator++) {
        authDB->addUserRecord((auth_iterator->first).c_str(), (auth_iterator->second).c_str());
    }

    streamServer = RTSPStreamServer::createNew(*env,
                                               param_->payload_type_,
                                               param->bitrate_,
                                               queue,
                                               param_->port_,
                                               authDB);
    runtimeAssert("RTSPStreamServer::createNew failed", nullptr != streamServer);

    auto streamInput = RTSPStreamInput::createNew(*env, queue, param_->stream_name_);
    streamServer->addStream(streamInput);

    log->infof("[%s#%d]RTSPServer created successful!", __func__, __LINE__);
}

RTSPService::~RTSPService() {
//    teardown();     /* all calls to LIVE555 code *must* be done within the LIVE555 thread */
}

void RTSPService::teardown() {
    if (nullptr != streamServer) {
        streamServer->removeStream(param_->stream_name_);
        Medium::close(reinterpret_cast<Medium *>(streamServer));  // deletes all server media sessions
        streamServer = nullptr;
    }
    //serverMediaSession->deleteAllSubsessions(); /* `Medium::close(rtspServer);` will automatically delete any `serverMediaSession` object */

    if (nullptr != authDB) {
        delete (authDB);
        authDB = nullptr;
    }

    if (nullptr != env) {
        env->reclaim();
        env = nullptr;
    }

    if (nullptr != taskScheduler) {
        delete (taskScheduler);
        taskScheduler = nullptr;
    }
    log->infof("[%s#%d]%s", __func__, __LINE__, service_name.c_str());
}

bool RTSPService::init() {
    start();
    return true;
}

void RTSPService::destroy() {
    terminate_event_loop = true;     /* interrupt the event loop, and teardown */
    stop();
    teardown();
    log->infof("[%s#%d]%s", __func__, __LINE__, service_name.c_str());
}

bool RTSPService::threadLoop() {
    /* "triggerEvent()" function is the *only* LIVE555 function that can be called from an external thread */
    env->taskScheduler().doEventLoop(&terminate_event_loop);
    log->infof("%s:terminate_event_loop:%d", service_name.c_str(), terminate_event_loop);
    return false; // break
}

bool RTSPService::increaseSocketBufferMaxSize(uint64_t buf_size) {
#if defined(__linux__)
    std::string send_socket = "/proc/sys/net/core/wmem_max";
    std::string recv_socket = "/proc/sys/net/core/rmem_max";

    writeValue(send_socket, buf_size);
    writeValue(recv_socket, buf_size);
#endif
    return true;
}

bool RTSPService::writeValue(std::string fname, uint64_t value) {
    int32_t fd = open(fname.c_str(), O_WRONLY | O_TRUNC, 0777);
    if (fd < 0) {
        log->errorf("[%s#%d]open failed, %s", __func__, __LINE__, strerror(errno));
        return false;
    }

    if (write(fd, &value, sizeof(value)) < 0) {
        log->errorf("[%s#%d]write failed, %s", __func__, __LINE__, strerror(errno));
        return false;
    }

    if (close(fd) < 0) {
        log->errorf("[%s#%d]close failed, %s", __func__, __LINE__, strerror(errno));
        return false;
    }
    return true;
}