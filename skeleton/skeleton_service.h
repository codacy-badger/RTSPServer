#ifndef PROJECT_SKELETON_SERVICE_H
#define PROJECT_SKELETON_SERVICE_H

#include <boost/thread.hpp>
#include "utils/log.h"

class SkeletonService {
public:
    SkeletonService();

    virtual void start();

    virtual void stop();

    void join();

protected:
    virtual bool threadLoop()= 0;

    void setThreadName(std::string name);

protected:
    boost::thread thread;

    std::string service_name;

    Log *log;

private:
    void run();

    std::string getDemangleName();
};

#endif //PROJECT_SKELETON_SERVICE_H
