#ifndef PROJECT_FILE_H
#define PROJECT_FILE_H

#include <string>
#include <cstdint>
#include <sys/types.h>
#include <fcntl.h>

class File {
public:
    static ssize_t write(std::string fname, void *buf, size_t len, int32_t flag = O_CREAT | O_WRONLY | O_TRUNC);

    static ssize_t read(std::string fname, void *buf, size_t len);

    static off_t length(std::string fname);

    static bool exist(std::string fname);

    static bool createDir(std::string dir_name);
};

#endif //PROJECT_FILE_H
