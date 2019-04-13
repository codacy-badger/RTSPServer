#ifndef PROJECT_VIDEO_CODEC_PARAMETER_H
#define PROJECT_VIDEO_CODEC_PARAMETER_H

extern "C" {
#include <libavcodec/avcodec.h>
}

struct VideoCodecParameter {
    int32_t stream_index_;

    AVCodecID codec_id_;

    int32_t width_;

    int32_t height_;

    int32_t bitrate_;

    int32_t gop_;

    int32_t fps_;
};

#endif //PROJECT_VIDEO_CODEC_PARAMETER_H
