#ifndef PROJECT_RTSP_PARAMETER_H
#define PROJECT_RTSP_PARAMETER_H

#include <map>
#include <cstdint>
#include <string>

struct RTSPParameter {
    std::string stream_name_;

    uint32_t port_;

    int32_t payload_type_;

    int32_t bitrate_;

    std::map<std::string, std::string> account;
};

#endif //PROJECT_RTSP_PARAMETER_H
