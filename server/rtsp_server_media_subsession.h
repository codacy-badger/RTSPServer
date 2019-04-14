#ifndef PROJECT_RTSP_SERVER_MEDIA_SUBSESSION_H
#define PROJECT_RTSP_SERVER_MEDIA_SUBSESSION_H

#include "OnDemandServerMediaSubsession.hh"
#include "FramedFilter.hh"
#include "Groupsock.hh"
#include "utils/stream_queue.h"
#include "server/rtsp_stream_input.h"

class RTSPServerMediaSubsession : public OnDemandServerMediaSubsession {
public:
    static RTSPServerMediaSubsession *createNew(int32_t payload_type,
                                                int32_t bitrate,
                                                std::shared_ptr<StreamQueue> streamQueue,
                                                RTSPStreamInput *streamInput,
                                                UsageEnvironment &env,
                                                Boolean reuseFirstSource = True);

protected:
    virtual FramedSource *createNewStreamSource(unsigned clientSessionId,
                                                unsigned &estBitrate);

    virtual RTPSink *createNewRTPSink(Groupsock *rtpGroupsock,
                                      unsigned char rtpPayloadTypeIfDynamic,
                                      FramedSource *inputSource);

    RTSPServerMediaSubsession(int32_t payload_type,
                              int32_t bitrate,
                              std::shared_ptr<StreamQueue> streamQueue,
                              RTSPStreamInput *streamInput,
                              UsageEnvironment &env,
                              Boolean reuseFirstSource);

    virtual ~RTSPServerMediaSubsession();

    virtual void closeStreamSource(FramedSource *inputSource);

private:
    std::shared_ptr<StreamQueue> streamQueue_;

    int32_t payload_type_;

    uint32_t max_size_;

    unsigned preferredPacketSize;

    unsigned maxPacketSize;

    int32_t bitrate_;

    RTSPStreamInput *streamInput_;

    FramedFilter *streamSource_;
};

#endif //PROJECT_RTSP_SERVER_MEDIA_SUBSESSION_H
