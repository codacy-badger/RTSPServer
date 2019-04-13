#ifndef PROJECT_RECORD_SERVICE_H
#define PROJECT_RECORD_SERVICE_H

#include "skeleton/skeleton_service.h"
#include "client/stream_record.h"
#include "client/video_codec_parameter.h"
#include "utils/date_time.h"
#include "utils/stream_queue.h"

class RecordService : public SkeletonService {
public:
    RecordService();

    ~RecordService() = default;

    bool init(std::shared_ptr<VideoCodecParameter> param);

    void destory();

    void setStreamQueue(std::shared_ptr<StreamQueue> queue);

protected:
    virtual bool threadLoop();

private:
    void createNewFileIfNecesarry(std::shared_ptr<AVPacket> packet);

private:
    std::shared_ptr<StreamQueue> stream_queue_;

     std::shared_ptr<StreamRecord> stream_record_;

    Time *time_;

    uint64_t first_pts_;

    int32_t duration_min;

    std::string fname_;

    AVFormatContext *fmt_ctx_;

    std::string record_dir;
};

#endif //PROJECT_RECORD_SERVICE_H
