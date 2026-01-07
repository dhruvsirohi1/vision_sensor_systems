//
// Created by Dhruv Sirohi on 1/2/26.
//

#ifndef TIME_SYNCHRONIZER_HPP
#define TIME_SYNCHRONIZER_HPP

#include "ring_buffer.hpp"
#include "camera_frame.hpp"
#include "imu_measurement.hpp"
#include "synchronized_packet.hpp"

#include <deque>
#include <mutex>

class TimeSynchronizer {
public:
    TimeSynchronizer(RingBuffer<CameraFrame>& cam_buffer,
        RingBuffer<ImuMeasurement>& imu_buffer,
        RingBuffer<SynchronizedPacket>& synchronized_buffer);

    ~TimeSynchronizer();

    void process();

private:

    bool interpIMU(uint64_t target_ts, ImuMeasurement& out);
    RingBuffer<CameraFrame>& camera_buffer_;
    RingBuffer<ImuMeasurement>& imu_buffer_;
    RingBuffer<SynchronizedPacket>& synchronized_buffer_;
    std::deque<ImuMeasurement> imu_window_;

    std::mutex imu_mtx_;
    std::condition_variable cv_;
};
#endif //TIME_SYNCHRONIZER_HPP
