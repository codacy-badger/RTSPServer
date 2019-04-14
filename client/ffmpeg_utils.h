#ifndef PROJECT_FFMPEG_UTILS_H
#define PROJECT_FFMPEG_UTILS_H

#include <map>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}
#include "utils/log.h"

class FFmpegUtils {
public:
    FFmpegUtils();

    virtual ~FFmpegUtils();

    virtual bool openStream(std::string url) = 0;

    virtual bool closeStream(std::string url) = 0;

    virtual int32_t findStream(AVFormatContext *avformat_ctx, AVMediaType media_type, std::string url);

    virtual std::shared_ptr<AVPacket> recvStream(AVFormatContext *avformat_ctx, int32_t stream_index);

    virtual bool dumpPacket(std::string url, AVPacket *packet);

    static inline bool keyframe(std::shared_ptr<AVPacket> packet) { return static_cast<bool>((packet->flags & AV_PKT_FLAG_KEY)); }

protected:
    Log *log_;
};

#endif //PROJECT_FFMPEG_UTILS_H
