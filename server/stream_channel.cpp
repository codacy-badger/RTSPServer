#include "server/stream_channel.h"

StreamChannel *StreamChannel::createNew(StreamType type) {
    return new StreamChannel(type);
}

StreamChannel::StreamChannel(StreamType type) : stream_type_(type) {
    if (StreamType::kSlave == type) {
        encoder_channel_ = ChannelType::kVideoEncodeLowQuality;
    } else {
        encoder_channel_ = ChannelType::kVideoEncodeHighQuality;
    }
}

int32_t StreamChannel::channelNo() {
    return static_cast<int32_t >(encoder_channel_);
}
