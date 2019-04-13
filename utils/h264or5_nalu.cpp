//
// Created by kang on 2019/3/6.
//

#include "utils/h264or5_nalu.h"

int32_t H264or5NALU::start_code_offset(void *buf, uint32_t len) {
    int32_t offset = -1;
    auto data = reinterpret_cast<uint8_t *>(buf);
    if (nullptr != data && len > 4) {
        for (uint32_t index = 0; index < (len - 4); index++) {
            auto tmp = reinterpret_cast<uint32_t *>(data + index);
            if (0x010000 == *tmp) {
                offset = index;
                break;
            } else if (0x01000000 == *tmp) {
                offset = index;
                break;
            }
        }
    }
    return offset;
}

int32_t H264or5NALU::start_code_length(void *buf, uint32_t len) {
    int32_t start_code_len = 0;
    auto data = reinterpret_cast<uint32_t *>(buf);
    if (nullptr != data && len > 4) {
        if (0x010000 == data[0]) {
            start_code_len = 3;
        } else if (0x01000000 == data[0]) {
            start_code_len = 4;
        }
    }
    return start_code_len;
}
