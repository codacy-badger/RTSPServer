#include "skeleton/exception.h"
#include "utils/file.h"
#include "client/record_service.h"

using namespace exception;

RecordService::RecordService()
    : fmt_ctx_(nullptr),
      stream_record_(nullptr),
      stream_queue_(nullptr),
      first_pts_(0),
      duration_min(10),
      record_dir("./record") {
    time_ = Time::getInstance();
    File::createDir(record_dir);
}

bool RecordService::init(std::shared_ptr<VideoCodecParameter> param) {
    runtimeAssert("RecordService stream_queue_ not initiated!", nullptr != stream_queue_);
    stream_record_ = std::make_shared<StreamRecord>(param);
    start();
    return true;
}

void RecordService::destory() {
    stop();
    stream_record_->closeStream(fname_);
}

bool RecordService::threadLoop() {
    auto packet = stream_queue_->dequeue();
    if (nullptr == packet) {
        boost::this_thread::sleep(boost::posix_time::milliseconds(15));
        return true;
    }
    createNewFileIfNecesarry(packet);
    if (File::exist(fname_)) {
        stream_record_->writeVideoFrame(packet);
    } else {
        log->errorf("[%s#%d]RecordService create new file failed, %s!!!", __func__, __LINE__, fname_.c_str());
        return false;
    }
    return true;
}

void RecordService::createNewFileIfNecesarry(std::shared_ptr<AVPacket> packet) {
    int64_t duration_ms = duration_min * 60;
    auto extension = std::string(".mp4");

    static bool success = false;
    if (nullptr != packet) {
        auto ts_now = time_->getTimestamp();
        if (ts_now - first_pts_ >= duration_ms) {
            auto date_str = Time::timestamp_to_string(ts_now);
            fname_ = record_dir + std::string("/") + date_str + extension;
            if (fname_.empty()) {
                return ;
            }

            if (success) {
                stream_record_->closeStream(fname_);
            }
            success = stream_record_->openStream(fname_);
            if (success) {
                first_pts_ = ts_now;
            }
        }
    }
}

void RecordService::setStreamQueue(std::shared_ptr<StreamQueue> queue) {
    stream_queue_ = queue;
}
