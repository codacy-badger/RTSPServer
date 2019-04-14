#include "GroupsockHelper.hh"
#include "H264VideoRTPSink.hh"
#include "H264VideoStreamDiscreteFramer.hh"
#include "H265VideoRTPSink.hh"
#include "H265VideoStreamDiscreteFramer.hh"
#include "server/rtsp_server_media_subsession.h"

RTSPServerMediaSubsession *RTSPServerMediaSubsession::createNew(int32_t payload_type,
                                                                int32_t bitrate,
                                                                std::shared_ptr<StreamQueue> streamQueue,
                                                                RTSPStreamInput *streamInput,
                                                                UsageEnvironment &env,
                                                                Boolean reuseFirstSource) {
    return new RTSPServerMediaSubsession(payload_type, bitrate, streamQueue, streamInput, env, reuseFirstSource);
}

RTSPServerMediaSubsession::RTSPServerMediaSubsession(int32_t payload_type,
                                                     int32_t bitrate,
                                                     std::shared_ptr<StreamQueue> streamQueue,
                                                     RTSPStreamInput *streamInput,
                                                     UsageEnvironment &env,
                                                     Boolean reuseFirstSource)
    : OnDemandServerMediaSubsession(env, reuseFirstSource),
      preferredPacketSize(1000),
      maxPacketSize(8192),
      streamQueue_(streamQueue),
      payload_type_(payload_type),
      streamInput_(streamInput),
      streamSource_(nullptr),
      max_size_(2*1024*1024),
      bitrate_(bitrate) {
}

RTSPServerMediaSubsession::~RTSPServerMediaSubsession() {

}

FramedSource *RTSPServerMediaSubsession::createNewStreamSource(unsigned clientSessionId,
                                                               unsigned &estBitrate) {
    estBitrate = bitrate_;
    if (nullptr != streamSource_) {
        auto input_source = dynamic_cast<RTSPLiveSource *>(streamSource_->inputSource());
        input_source->referenceCount()++;
        return reinterpret_cast<FramedSource *>(streamSource_);
    }

    if (payload_type_ == AV_CODEC_ID_H264) {
        streamSource_ = H264VideoStreamDiscreteFramer::createNew(envir(), streamInput_->videoSource(streamQueue_));
    } else if (payload_type_ == AV_CODEC_ID_H265) {
        streamSource_ = H265VideoStreamDiscreteFramer::createNew(envir(), streamInput_->videoSource(streamQueue_));
    }
    auto input_source = dynamic_cast<RTSPLiveSource *>(streamSource_->inputSource());
    input_source->firstConnect() = true;

    return reinterpret_cast<FramedSource *>(streamSource_);
}

RTPSink *RTSPServerMediaSubsession::createNewRTPSink(Groupsock *rtpGroupsock,
                                                     unsigned char rtpPayloadTypeIfDynamic,
                                                     FramedSource *inputSource) {
    H264or5VideoRTPSink *rtpSink = nullptr;
    OutPacketBuffer::increaseMaxSizeTo(max_size_);  /* packet-loss frequently */
    if (payload_type_ == AV_CODEC_ID_H264) {
        rtpSink = H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
    } else if (payload_type_ == AV_CODEC_ID_H265) {
        rtpSink = H265VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
    } else {
        rtpSink = nullptr;
    }

    if (nullptr != rtpSink) {
        rtpSink->setPacketSizes(preferredPacketSize, maxPacketSize);
    }
    return reinterpret_cast<RTPSink *>(rtpSink);
}

void RTSPServerMediaSubsession::closeStreamSource(FramedSource *inputSource) {
    if (inputSource != reinterpret_cast<FramedSource *>(streamSource_)) { /* Sanity check, should not happend */
        Medium::close(reinterpret_cast<Medium *>(inputSource));
    }

    auto liveSource = dynamic_cast<RTSPLiveSource *>(streamSource_->inputSource());
    if (0 == (--liveSource->referenceCount())) {
        Medium::close(reinterpret_cast<Medium *>(streamSource_));
        streamSource_ = nullptr;
    }
}