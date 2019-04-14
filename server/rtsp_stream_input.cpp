#include "server/rtsp_stream_input.h"

RTSPStreamInput *RTSPStreamInput::createNew(UsageEnvironment &env,
                                            std::shared_ptr<StreamQueue> queue,
                                            std::string streamName) {
    return new RTSPStreamInput(env, queue, streamName);
}

RTSPStreamInput::RTSPStreamInput(UsageEnvironment &env, std::shared_ptr<StreamQueue> queue, std::string streamName)
    : Medium(env), fStreamQueue(queue), fVideoSource(nullptr), fStreamName(streamName) {
}

RTSPStreamInput::~RTSPStreamInput() {
    if (nullptr != fVideoSource) {
        RTSPLiveSource::handleClosure(fVideoSource);
    }
}

FramedSource *RTSPStreamInput::videoSource(std::shared_ptr<StreamQueue> queue) {
    return (fVideoSource = RTSPLiveSource::createNew(fStreamQueue, envir()));
}