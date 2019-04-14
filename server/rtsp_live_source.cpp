#include "utils/h264or5_nalu.h"
#include "client/ffmpeg_utils.h"
#include "server/rtsp_live_source.h"

RTSPLiveSource::RTSPLiveSource(std::shared_ptr<StreamQueue> streamQueue, UsageEnvironment &env)
    : FramedSource(env), streamQueue_(streamQueue), fReferenceCount(1), firstClientConnect_(false) {
    log_ = Log::getInstance();
}

RTSPLiveSource::~RTSPLiveSource() {
}

RTSPLiveSource *RTSPLiveSource::createNew(std::shared_ptr<StreamQueue> streamQueue, UsageEnvironment &env) {
    return new RTSPLiveSource(streamQueue, env);
}

void RTSPLiveSource::waitIDRIfFirstConnect(std::shared_ptr<AVPacket> packet) {
    if (nullptr != packet && firstConnect()) {
        do {
            if (FFmpegUtils::keyframe(packet)) {
                firstConnect() = false;
                break;
            }

            packet = streamQueue_->dequeue();
            if (nullptr == packet) {
                log_->tracef("[%s#%d]empty queue or invalid payload format!!!", __func__, __LINE__);
                return;
            }
        } while (1);
    }
}

void RTSPLiveSource::doGetNextFrame() {
    static struct timeval tv_prev = {.tv_sec = 0, .tv_usec = 0};
    struct timeval tv_now;
    gettimeofday(&tv_now, nullptr);
    double diff_us = (tv_now.tv_sec - tv_prev.tv_sec)*1000000 + (tv_now.tv_usec - tv_prev.tv_usec);
    tv_prev = tv_now;
    log_->tracef("[%s#%d]to get frame, diff_ms:%.2f !!!", __func__, __LINE__, diff_us/1000);
    auto packet = streamQueue_->dequeue();
    fFrameSize = 0;                    // reset frame size
    fDurationInMicroseconds = 0;    // set to zero for live source, non-zero for file source
    if (packet == nullptr) {
        log_->warningf("[%s#%d]The frame queue is empty!", __func__, __LINE__);
        return;
    }
    log_->tracef("[%s#%d]pts:%llu, len:%llu", __func__, __LINE__, packet->pts, packet->size);

    waitIDRIfFirstConnect(packet);
    if (packet->size > fMaxSize) {
        fFrameSize = fMaxSize;
        fNumTruncatedBytes = packet->size - fMaxSize;
        log_->infof("[%s#%d]packet length(%llu) exceed the fMaxSize(%u), fNumTruncatedBytes(%u)!!!",
                    __func__,
                    __LINE__,
                    packet->size,
                    fMaxSize,
                    fNumTruncatedBytes);
    } else {
        fFrameSize = packet->size;
        fNumTruncatedBytes = 0;
    }

    fPresentationTime.tv_sec = packet->pts/1000000UL;
    fPresentationTime.tv_usec = packet->pts%1000000UL;

    auto offset = H264or5NALU::start_code_offset(packet->data, fFrameSize);
    auto startcode_length = H264or5NALU::start_code_length(packet->data + offset, fFrameSize - offset);
    auto total_offset = offset + startcode_length;
    fFrameSize -= total_offset;
    memcpy(fTo, packet->data + total_offset, fFrameSize);
    if (fFrameSize < 1) {
        log_->warningf("[%s#%d]illegal fFrameSize(%d)", __func__, __LINE__, fFrameSize);
    }

    FramedSource::afterGetting(this);   /* read from a live source efficiently, and avoid infinite recursion !!! */
}

unsigned RTSPLiveSource::maxFrameSize() const {
    // By default, this source has no maximum frame size.
    return 150000;
}
