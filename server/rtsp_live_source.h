#ifndef PROJECT_RTSP_LIVE_SOURCE_H
#define PROJECT_RTSP_LIVE_SOURCE_H

#include <map>
#include "FramedSource.hh"
#include "utils/stream_queue.h"
#include "utils/log.h" // NOLINT

class RTSPLiveSource : public FramedSource {
public:
    static RTSPLiveSource *createNew(std::shared_ptr<StreamQueue> streamQueue, UsageEnvironment &env);

    int32_t &referenceCount() { return fReferenceCount; }

    bool &firstConnect() { return firstClientConnect_; }

protected:
    RTSPLiveSource(std::shared_ptr<StreamQueue> streamQueue, UsageEnvironment &env);

    ~RTSPLiveSource();

private:
    virtual void doGetNextFrame();

    virtual unsigned maxFrameSize() const;

    void waitIDRIfFirstConnect(std::shared_ptr<AVPacket> packet);

private:
    std::shared_ptr<StreamQueue> streamQueue_;

    int32_t fReferenceCount;

    Log *log_;

    bool firstClientConnect_;
};

#endif //PROJECT_RTSP_LIVE_SOURCE_H
