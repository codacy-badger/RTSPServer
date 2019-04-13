#include "client/stream_transcode.h"

bool StreamTranscode::openStream(std::string url) {
    AVFormatContext *iformat_ctx = nullptr;
    AVDictionary *opts = nullptr;
    bool use_udp = true;
    const int64_t sec = 1000*1000;
    const int32_t kMaxOpenTimes = 2;
    int32_t ret = 0;

    if (!av_dict_get(opts, "scan_all_pmts", nullptr, AV_DICT_MATCH_CASE)) {
        av_dict_set(&opts, "scan_all_pmts", "1", AV_DICT_DONT_OVERWRITE);
    }
    av_dict_set_int(&opts, "stimeout", 10*sec, 0);
    av_dict_set_int(&opts, "probesize", 5000000*3, 0);

    int32_t try_open_times = 0;
    do {
        std::string rtsp_transport = use_udp ? "udp" : "tcp";
        av_dict_set(&opts, "rtsp_transport", rtsp_transport.c_str(), 0);
        if (0 == (ret = avformat_open_input(&iformat_ctx, url.c_str(), nullptr, &opts))) {
            log_->infof("[%s#%d]open %s successfully!", __func__, __LINE__, url.c_str());
            break;
        }
        use_udp = !use_udp;
        if (++try_open_times < kMaxOpenTimes) {
            log_->warningf("[%s#%d]open %s failed, rc = %d, try connect with %s.",
                           __func__,
                           __LINE__,
                           url.c_str(),
                           ret,
                           use_udp ? "udp" : "tcp");
        }
    } while (try_open_times < kMaxOpenTimes);

    if (ret != 0) {
        goto open_failed_exit_label;
    }

    if ((ret = avformat_find_stream_info(iformat_ctx, nullptr)) < 0) {
        log_->errorf("[%s#%d]avformat_find_stream_info failed, ret = %d", __func__, __LINE__, ret);
        goto find_stream_failed_exit_label;
    }
    stream_table_[url] = iformat_ctx;
    av_dict_free(&opts);
    return true;

find_stream_failed_exit_label:
    closeStream(url);

open_failed_exit_label:
    av_dict_free(&opts);
    return false;
}

bool StreamTranscode::closeStream(std::string url) {
    auto it = stream_table_.find(url);
    if (it == stream_table_.end()) {
        log_->errorf("[%s#%d]url:%s", __func__, __LINE__, url.c_str());
        return false;
    }
    auto iformat_ctx = it->second;
    avformat_close_input(&iformat_ctx);
    return (nullptr == iformat_ctx);
}

std::shared_ptr<VideoCodecParameter> StreamTranscode::getCodecParameter(std::string url) {
    auto it = stream_table_.find(url);
    if (it == stream_table_.end()) {
        log_->errorf("[%s#%d]url:%s", __func__, __LINE__, url.c_str());
        return nullptr;
    }

    auto iformat_ctx = it->second;
    if (nullptr == iformat_ctx) {
        log_->errorf("[%s#%d]iformat_ctx nullptr", __func__, __LINE__);
        return nullptr;
    }

    auto stream_index = findStream(iformat_ctx, AVMEDIA_TYPE_VIDEO, url);
    if (stream_index < 0) {
        log_->errorf("[%s#%d]findStream failed, stream_index = %d!", __func__, __LINE__, stream_index);
        return nullptr;
    }

    AVStream *avstream = iformat_ctx->streams[stream_index];
    param_->stream_index_ = stream_index;
    param_->width_ = avstream->codecpar->width;
    param_->height_ = avstream->codecpar->height;
    param_->bitrate_ = avstream->codecpar->bit_rate;
    param_->fps_ = avstream->avg_frame_rate.num/static_cast<double >(avstream->avg_frame_rate.den);
    auto codec_id = avstream->codecpar->codec_id;
    if (AV_CODEC_ID_H264 != codec_id && AV_CODEC_ID_H265 != codec_id) {
        log_->errorf("[%s#%d]decoder not support suche payload type!", __func__, __LINE__);
        return nullptr;
    }
    param_->codec_id_ = codec_id;
    return param_;
}

std::shared_ptr<AVPacket> StreamTranscode::recvStream(std::string url) {
    auto it = stream_table_.find(url);
    if (it == stream_table_.end()) {
        log_->errorf("[%s#%d]url:%s may not be opened successfully!!!", __func__, __LINE__, url);
        return nullptr;
    }
    auto iformat_ctx = it->second;
    if (nullptr == iformat_ctx) {
        log_->errorf("[%s#%d]url:%s may not be opened successfully!!!", __func__, __LINE__, url);
        return nullptr;
    }
    return FFmpegUtils::recvStream(iformat_ctx, param_->stream_index_);
}
