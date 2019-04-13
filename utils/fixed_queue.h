#ifndef PROJECT_FIXED_QUEUE_H
#define PROJECT_FIXED_QUEUE_H

#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <boost/timer/timer.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>
#include "utils/log.h"

template <typename T>
class FixedQueue {
public:
    FixedQueue(uint32_t capacity = 5) : capacity_(capacity) { log_ = Log::getInstance(); };

    virtual ~FixedQueue()  { clear(); }

    void enqueue(std::shared_ptr<T> frame);

    std::shared_ptr<T> dequeue();

    void clear();

    uint32_t size() { return queue_.size(); }

private:
    int  capacity_;

    std::mutex  lock_;

    std::condition_variable  cond_;

    std::queue<std::shared_ptr<T>> queue_;

    Log*  log_;
};

template<typename T>
void FixedQueue<T>::enqueue(std::shared_ptr<T> entry)
{
    std::unique_lock<std::mutex> queue_lock(lock_);

    while (queue_.size() >= capacity_) {
        queue_.front();
        queue_.pop();
    }
    queue_.push(entry);
    cond_.notify_one();
}

template<typename T>
std::shared_ptr<T> FixedQueue<T>::dequeue()
{
    std::unique_lock<std::mutex> queue_lock(lock_);

    cond_.wait(queue_lock, [&]{ return !queue_.empty(); });
    auto entry = queue_.front();
    queue_.pop();

    return entry;
}

template <typename T>
void FixedQueue<T>::clear() {
    while (size() > 0) {
        dequeue();
    }
}

#endif //PROJECT_FIXED_QUEUE_H
