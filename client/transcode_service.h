#ifndef PROJECT_TRANSCODE_SERVICE_H
#define PROJECT_TRANSCODE_SERVICE_H

#include "skeleton/skeleton_service.h"
#include "utils/stream_queue.h"
#include "client/stream_transcode.h"

class TranscodeService : public SkeletonService {
public:
    TranscodeService();

    ~TranscodeService() = default;

    bool init(std::string url);

    void destory();

    void setStreamQueue(std::shared_ptr<StreamQueue> queue);

    std::shared_ptr<VideoCodecParameter> getCodecParameter();

protected:
    bool threadLoop();

private:
    std::string url_;

    std::shared_ptr<StreamTranscode> stream_transcode_;

    std::shared_ptr<VideoCodecParameter> param_;

    std::shared_ptr<StreamQueue> stream_queue_;
};

#endif //PROJECT_TRANSCODE_SERVICE_H
