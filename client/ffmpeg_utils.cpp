extern "C" {
#include <libavutil/dict.h>
}

#include "skeleton/exception.h"
#include "utils/file.h"
#include "client/ffmpeg_utils.h"

FFmpegUtils::FFmpegUtils() {
    log_ = Log::getInstance();
//    av_register_all();        /* deprecated */
    avformat_network_init();
}

FFmpegUtils::~FFmpegUtils() {
    avformat_network_deinit();
}

std::shared_ptr<AVPacket> FFmpegUtils::recvStream(AVFormatContext *avformat_ctx, int32_t stream_index) {
    if (nullptr == avformat_ctx) {
        log_->errorf("[%s#d]avformat_ctx is nullptr!!!", __func__, __LINE__);
        return nullptr;
    }

    std::shared_ptr<AVPacket> avpacket = std::make_shared<AVPacket>();
    int32_t ret = 0;

    av_init_packet(avpacket.get());
    ret = av_read_frame(avformat_ctx, avpacket.get());
    if (EOF == ret) {
        log_->infof("[%s#%d]av_read_frame reach the end of stream !!!", __func__, __LINE__);
    } else if (ret < 0) {
        log_->errorf("[%s#%d]av_read_frame failed, ret = %d !!!", __func__, __LINE__, ret);
        goto exit_lable;
    }

    if (avpacket->stream_index != stream_index) {
        log_->tracef("[%s#%d]skip the stream not needed, stream_index:%d", __func__, __LINE__, avpacket->stream_index);
        goto exit_lable;
    }
    return avpacket;

exit_lable:
    av_packet_unref(avpacket.get());
    return nullptr;
}

int32_t FFmpegUtils::findStream(AVFormatContext *avformat_ctx, AVMediaType media_type, std::string url) {
    AVCodec *codec = nullptr;
    int32_t ret;
    if ((ret = av_find_best_stream(avformat_ctx, media_type, -1, -1, &codec, 0)) < 0) {
        if (AVERROR_DECODER_NOT_FOUND == ret || AVERROR_STREAM_NOT_FOUND == ret) {
            log_->errorf("[%s#%d]av_find_best_stream failed,!!!", __func__, __LINE__);
        } else {
            log_->errorf("[%s#%d]av_find_best_stream failed, ret = %d", __func__, __LINE__, ret);
        }
        return -1;
    }
    av_dump_format(avformat_ctx, ret, url.c_str(), 0);
    return ret;
}

bool FFmpegUtils::dumpPacket(std::string url, AVPacket *packet) {
    if (url.empty() || nullptr == packet) {
        log_->errorf("[%s#%d]invalid parameter!!!", __func__, __LINE__);
        return false;
    }

    if (File::write(url, packet->data, packet->size, O_CREAT | O_WRONLY | O_APPEND) < 0) {
        log_->errorf("[%s#%d]write failed!", __func__, __LINE__);
        return false;
    }
    return true;
}