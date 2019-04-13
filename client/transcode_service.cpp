#include "skeleton/exception.h"
#include "client/transcode_service.h"

using namespace exception;

TranscodeService::TranscodeService() : url_(""), param_(nullptr) {
    stream_transcode_ = std::make_shared<StreamTranscode>();
}

bool TranscodeService::init(std::string url) {
    runtimeAssert("TranscodeService stream_queue_ not initiated!", nullptr != stream_queue_);
    url_ = url;
    if (!stream_transcode_->openStream(url_)) {
        log->errorf("[%s#%d]%s openStream %s failed!!!", __func__, __LINE__, service_name.c_str(), url_.c_str());
        return false;
    }

    if (nullptr == (param_ = stream_transcode_->getCodecParameter(url_))) {
        log->errorf("[%s#%d]%s getCodecParameter failed, url:%s!!!", __func__, __LINE__, service_name.c_str(), url_.c_str());
        return false;
    }
    start();
    return true;
}

void TranscodeService::destory() {
    stop();
    if (!stream_transcode_->closeStream(url_)) {
        log->errorf("[%s#%d]%s closeStream %s failed!!!", __func__, __LINE__, service_name.c_str(), url_.c_str());
        return ;
    }
}

bool TranscodeService::threadLoop() {
    auto packet = stream_transcode_->recvStream(url_);
    if (nullptr == packet) {
        boost::this_thread::sleep(boost::posix_time::milliseconds(15));
        return true;
    }
    stream_queue_->enqueue(packet);
    av_packet_unref(packet.get());
    return true;
}

void TranscodeService::setStreamQueue(std::shared_ptr<StreamQueue> queue) {
    stream_queue_ = queue;
}

std::shared_ptr<VideoCodecParameter> TranscodeService::getCodecParameter() {
    return param_;
}
