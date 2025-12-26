//
// Created by Dhruv Sirohi on 12/25/25.
//

#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP
#include <deque>

template<typename T>
class RingBuffer {
public:
    explicit RingBuffer(int capacity) :
    capacity_(capacity) {}

    RingBuffer(const RingBuffer&) = default;

    void push(const T& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (buffer_.size() >= capacity_) {
            buffer_.pop_back();
        }
        buffer_.push_front(item);
        cv_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [&]{ return !buffer_.empty(); });
        T item = buffer_.front();
        buffer_.pop_front();
        return item;
    }


private:
    int capacity_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::deque<T> buffer_;
};
#endif //RING_BUFFER_HPP
