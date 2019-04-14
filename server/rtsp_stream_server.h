#ifndef PROJECT_RTSP_STREAM_SERVER_H
#define PROJECT_RTSP_STREAM_SERVER_H

#include "NetAddress.hh"
#include "RTSPServer.hh"
#include "utils/log.h"
#include "server/rtsp_stream_input.h"
#include "server/stream_channel.h"

class RTSPStreamServer : public RTSPServer {
public:
    static RTSPStreamServer *createNew(UsageEnvironment &env,
                                       int32_t payload_type,
                                       int32_t bitrate,
                                       std::shared_ptr<StreamQueue> queue,
                                       Port ourPort = 554,
                                       UserAuthenticationDatabase *authDatabase = nullptr,
                                       unsigned reclamationTestSeconds = 65);

    void addStream(RTSPStreamInput *streamInput);

    void removeStream(RTSPStreamInput *streamInput);

    void removeStream(std::string streamName);

protected:
    RTSPStreamServer(UsageEnvironment &env,
                     int32_t payload_type,
                     int32_t bitrate,
                     int32_t ourSocket,
                     std::shared_ptr<StreamQueue> queue,
                     Port ourPort = 554,
                     UserAuthenticationDatabase *authDatabase = nullptr,
                     unsigned reclamationTestSeconds = 65);

    virtual ~RTSPStreamServer();

    virtual ServerMediaSession *lookupServerMediaSession(char const *streamName, Boolean isFirstLookupInSession = True);

    static int32_t setUpOurSocket(UsageEnvironment &env, Port &port);

private:
    Log *log_;

    HashTable *fStreamTable;

    int32_t payload_type_;

    int32_t bitrate_;

    std::shared_ptr<StreamQueue> streamQueue_;
};

#endif //PROJECT_RTSP_STREAM_SERVER_H
