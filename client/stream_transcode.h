#ifndef PROJECT_STREAM_TRANSCODE_H
#define PROJECT_STREAM_TRANSCODE_H

#include "client/ffmpeg_utils.h"
#include "client/video_codec_parameter.h"

class StreamTranscode : public FFmpegUtils {
public:
    StreamTranscode() { param_ = std::make_shared<VideoCodecParameter>(); }

    virtual ~StreamTranscode() = default;

    virtual bool openStream(std::string url);

    virtual bool closeStream(std::string url);

    std::shared_ptr<VideoCodecParameter> getCodecParameter(std::string url);

    virtual std::shared_ptr<AVPacket> recvStream(std::string url);

private:
    std::shared_ptr<VideoCodecParameter> param_;

    std::map<std::string, AVFormatContext*> stream_table_;
};

#endif //PROJECT_STREAM_TRANSCODE_H
