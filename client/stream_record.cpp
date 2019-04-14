extern "C" {
#include <libavutil/opt.h>
}
#include "client/stream_record.h"

StreamRecord::StreamRecord(std::shared_ptr<VideoCodecParameter> param)
    : oformat(nullptr),
      ofmt_ctx(nullptr),
      out_video_codec_ctx(nullptr),
      video_codec(nullptr),
      out_video_stream(nullptr),
      param_(param),
      frame_cnt(0),
      open_sucess(false) {
    log_ = Log::getInstance();
}

inline AVCodecID StreamRecord::getCodecID() {
    return param_->codec_id_;
}

bool StreamRecord::codecInit() {
    if (nullptr == (out_video_codec_ctx = avcodec_alloc_context3(video_codec))) {
        log_->errorf("[%s#%d]avcodec_alloc_context3 failed!!!", __func__, __LINE__);
        return false;
    }
    out_video_codec_ctx->codec_id = oformat->video_codec;
    out_video_codec_ctx->bit_rate = param_->bitrate_;
    out_video_codec_ctx->width = param_->width_;
    out_video_codec_ctx->height = param_->height_;

    out_video_stream->time_base = (AVRational) {1, param_->fps_};
    out_video_codec_ctx->time_base = out_video_stream->time_base;
    out_video_codec_ctx->gop_size = param_->gop_;
    out_video_codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    if (out_video_codec_ctx->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
        out_video_codec_ctx->max_b_frames = 2;
    }
    if (out_video_codec_ctx->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
        out_video_codec_ctx->mb_decision = 2;
    }

    if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER) {
        out_video_codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    return true;
}

bool StreamRecord::addStream() {
    if (nullptr == (video_codec = avcodec_find_encoder(oformat->video_codec))) {
        log_->errorf("[%s#%d]avcodec_find_encoder failed!!!", __func__, __LINE__);
        return false;
    }

    if (nullptr == (out_video_stream = avformat_new_stream(ofmt_ctx, nullptr))) {
        log_->errorf("[%s#%d]avformat_new_stream failed!!!", __func__, __LINE__);
        return false;
    }
    out_video_stream->id = ofmt_ctx->nb_streams - 1;

    if (!codecInit()) {
        avcodec_free_context(&out_video_codec_ctx);
        log_->errorf("[%s#%d]avcodec_parameters_from_context failed!!!", __func__, __LINE__);
        return false;
    }
    return true;
}

bool StreamRecord::openVideoStream() {
    if (avcodec_open2(out_video_codec_ctx, video_codec, nullptr) < 0) {
        log_->errorf("[%s#%d]avcodec_open2 failed!!!", __func__, __LINE__);
        return false;
    }

    if (avcodec_parameters_from_context(out_video_stream->codecpar, out_video_codec_ctx) < 0) {
        log_->errorf("[%s#%d]avcodec_parameters_from_context failed!!!", __func__, __LINE__);
        return false;
    }
    return true;
}

bool StreamRecord::openStream(std::string url) {
    if (avformat_alloc_output_context2(&ofmt_ctx, nullptr, nullptr, url.c_str()) < 0 || nullptr == ofmt_ctx) {
        log_->errorf("[%s#%d]avformat_alloc_output_context2 failed!!!", __func__, __LINE__);
        return false;
    }
    oformat = ofmt_ctx->oformat;
    oformat->video_codec = getCodecID();

    if (!addStream()) {
        log_->errorf("[%s#%d]addStream failed!!!", __func__, __LINE__);
        return false;
    }

    if (!openVideoStream()) {
        log_->errorf("[%s#%d]openVideoStream failed!!!", __func__, __LINE__);
        return false;
    }
    av_dump_format(ofmt_ctx, 0, url.c_str(), 1);

    if (!(oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&ofmt_ctx->pb, url.c_str(), AVIO_FLAG_WRITE) < 0) {
            log_->errorf("[%s#%d]avio_open failed!!!", __func__, __LINE__);
            return false;
        }
    }

    if (avformat_write_header(ofmt_ctx, nullptr) < 0) {
        if (!(oformat->flags & AVFMT_NOFILE)) {
            avio_closep(&ofmt_ctx->pb);
        }
        log_->errorf("[%s#%d]avformat_write_header failed!!!", __func__, __LINE__);
        return false;
    }
    open_sucess = true;
    frame_cnt = 0;

    return true;
}

bool StreamRecord::closeStream(std::string url) {
    if (!open_sucess) {
        return true;
    }

    /* Write the trailer, if any. The trailer must be written before you
     * close the CodecContexts open when you wrote the header; otherwise
     * av_write_trailer() may try to use memory that was freed on
     * av_codec_close(). */
    av_write_trailer(ofmt_ctx);

    /* Close each codec. */
    avcodec_free_context(&out_video_codec_ctx);

    /* Close the output file. */
    if (!(oformat->flags & AVFMT_NOFILE)) {
        avio_closep(&ofmt_ctx->pb);
    }

    /* free the stream */
    avformat_free_context(ofmt_ctx);
    frame_cnt = 0;
    return true;
}

void StreamRecord::writeVideoFrame(std::shared_ptr<AVPacket> packet) {
    AVPacket out_packet;
    av_init_packet(&out_packet);

    auto calc_duration = AV_TIME_BASE/static_cast<double >(param_->fps_); //Duration between 2 frames (us)
    out_packet.pts = frame_cnt*calc_duration/(av_q2d(out_video_stream->time_base)*AV_TIME_BASE);
    out_packet.dts = out_packet.pts;
    out_packet.duration = calc_duration/(av_q2d(out_video_stream->time_base)*AV_TIME_BASE);
    out_packet.data = packet->data;
    out_packet.size = (nullptr == out_packet.data) ? 0 : packet->size;

    if (0 == frame_cnt) {
        if (!keyframe(packet)) {
            log_->debugf("[%s#%d]skip non-keyframe!!!", __func__, __LINE__);
            return;
        }
        out_packet.flags |= AV_PKT_FLAG_KEY;
    }

    out_packet.stream_index = out_video_stream->index;
    out_packet.stream_index = out_video_stream->index;
    out_packet.pos = -1;

    if (av_interleaved_write_frame(ofmt_ctx, &out_packet) < 0) {
        log_->errorf("[%s#%d]av_interleaved_write_frame failed!!!", __func__, __LINE__);
        return;
    }
    frame_cnt++;
}