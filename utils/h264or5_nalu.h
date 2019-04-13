#ifndef PROJECT_H264OR5_NALU_H
#define PROJECT_H264OR5_NALU_H

#include <cstdint>

class H264or5NALU {
public:
    static int32_t start_code_offset(void *buf, uint32_t len);

    static int32_t start_code_length(void *buf, uint32_t len);
};

#endif //PROJECT_H264OR5_NALU_H
