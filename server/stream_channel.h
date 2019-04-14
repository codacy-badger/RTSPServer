#ifndef PROJECT_STREAMCHANNEL_H
#define PROJECT_STREAMCHANNEL_H

#include <cstdint>

enum class StreamType { /* RTSP stream type */
    kMaster,
    kSlave
};

enum class ChannelType { /* Encoder channel type */
    kVideoEncodeLowQuality,
    kVideoEncodeHighQuality
};

class StreamChannel {
public:
    static StreamChannel *createNew(StreamType type);

    int32_t channelNo();

    ~StreamChannel() = default;

protected:
    StreamChannel(StreamType type);

private:
    StreamType stream_type_;

    ChannelType encoder_channel_;
};

#endif //PROJECT_STREAMCHANNEL_H
