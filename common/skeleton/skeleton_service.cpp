#include <typeinfo>
#include <cxxabi.h>
#include <pthread.h>
#include "skeleton/skeleton_service.h"

SkeletonService::SkeletonService() {
    log = Log::getInstance();
}

void SkeletonService::start()
{
    thread = boost::thread(&SkeletonService::run, this);
    service_name = getDemangleName();
}

void SkeletonService::stop()
{
    const int join_timeout = 2000;
    thread.interrupt();
    auto success = thread.try_join_for(boost::chrono::milliseconds(join_timeout));
    if (success) {
        log->infof("service %s stopped", service_name.c_str());
    } else {
        log->infof("service %s not stop in %dms", service_name.c_str(), join_timeout);
    }
}

void SkeletonService::join()
{
    thread.join();
}

void SkeletonService::run()
{
    if (!service_name.empty()) {
        setThreadName(service_name);
    }

    try {
        do {
            boost::this_thread::interruption_point();
            if (!threadLoop()) {
                break;
            }
        } while (true);
    } catch (boost::thread_interrupted const &interrupted) { // exception safe
        log->infof("%s interrupted!!!", service_name.c_str());
    }
}

std::string SkeletonService::getDemangleName() {
    int status;
    auto cls_name = abi::__cxa_demangle(typeid(*this).name(), 0, 0, &status);;
    std::string name = cls_name;
    ::free(reinterpret_cast<void *>(cls_name));
    return name;
}

void SkeletonService::setThreadName(std::string name) {
#if defined(__NetBSD__)
    pthread_setname_np(thread.native_handle(), "%s", name.c_str());
#elif defined(__APPLE__) && defined(__MACH__)
    pthread_setname_np(name.c_str());
// #elif defined(__linux__) || defined(__FreeBSD__)
#else
    pthread_setname_np(thread.native_handle(), name.c_str());
#endif
}
