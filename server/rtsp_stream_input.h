#ifndef PROJECT_RTSP_STREAM_INPUT_H
#define PROJECT_RTSP_STREAM_INPUT_H

#include <string>
#include "MediaSink.hh"
#include "UsageEnvironment.hh"
#include "server/rtsp_live_source.h"

class RTSPStreamInput : public Medium {
public:
    static RTSPStreamInput *createNew(UsageEnvironment &env,
                                      std::shared_ptr<StreamQueue> queue,
                                      std::string streamName);

    FramedSource *videoSource(std::shared_ptr<StreamQueue> queue);

    std::string streamName() const { return fStreamName; };

private:
    RTSPStreamInput(UsageEnvironment &env, std::shared_ptr<StreamQueue> queue, std::string streamName);

    virtual ~RTSPStreamInput();

private:
    FramedSource *fVideoSource;

    std::string fStreamName;

    std::shared_ptr<StreamQueue> fStreamQueue;
};

#endif //PROJECT_RTSP_STREAM_INPUT_H
