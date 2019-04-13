#ifndef PROJECT_STREAM_QUEUE_H
#define PROJECT_STREAM_QUEUE_H

extern "C" {
#include <libavcodec/avcodec.h>
}
#include "utils/fixed_queue.h"

using StreamQueue = FixedQueue<AVPacket>;

#endif //PROJECT_STREAM_QUEUE_H
