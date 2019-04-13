//
// Created by kang on 2019/3/1.
//

#include <boost/filesystem.hpp>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "utils/file.h"

ssize_t File::write(std::string fname, void *buf, size_t len, int32_t flag) {
    int32_t fd = -1;
    ssize_t result = -1;

    if (fname.empty() || nullptr == buf) {
        return -1;
    }

    if ((fd = ::open(fname.c_str(), flag, 0777)) < 0) {
        return -1;
    }

    if ((result = ::write(fd, buf, len)) < 0) {
        return result;
    }

    if (::close(fd) < 0) {
        return -1;
    }
    return result;
}

ssize_t File::read(std::string fname, void *buf, size_t len) {
    int32_t fd = -1;
    ssize_t result = -1;

    if (!exist(fname) || nullptr == buf) {
        return -1;
    }

    if ((fd = ::open(fname.c_str(), O_RDONLY, 0777)) < 0) {
        return -1;
    }

    if ((result = ::read(fd, buf, len)) < 0) {
        return result;
    }

    if (::close(fd) < 0) {
        return -1;
    }
    return result;
}

off_t File::length(std::string fname) {
    if (!exist(fname)) {
        return 0;
    }
    struct stat sbuf;
    memset(&sbuf, 0, sizeof(sbuf));
    if (stat(fname.c_str(), &sbuf) < 0) {
        return 0;
    }
    return sbuf.st_size;
}

bool File::exist(std::string fname) {
    if (fname.empty()) {
        return false;
    }
    return (0 == access(fname.c_str(), F_OK));
}

bool File::createDir(std::string dir_name) {
    if (boost::filesystem::exists(dir_name)) { // already created
        return true;
    }
    return boost::filesystem::create_directories(dir_name);
}