//
// Created by Dhruv Sirohi on 1/2/26.
//

#include "time_synchronizer.hpp"
#include <iostream>

TimeSynchronizer::TimeSynchronizer(
    RingBuffer<CameraFrame> &cam_buffer,
    RingBuffer<ImuMeasurement> &imu_buffer,
    RingBuffer<SynchronizedPacket> &synchronized_buffer) :
    camera_buffer_(cam_buffer),
    imu_buffer_(imu_buffer),
    synchronized_buffer_(synchronized_buffer) {}

SyncResult TimeSynchronizer::process() {
    CameraFrame cam_frame = camera_buffer_.pop();

    ImuMeasurement imu;
    if (!interpIMU(cam_frame.timestamp_ns, imu)) {
        return SyncResult::FAILED;
    }

    SynchronizedPacket synchronized_packet;
    synchronized_packet.timestamp_ns = cam_frame.timestamp_ns;
    synchronized_packet.imu = imu;
    synchronized_packet.camera = cam_frame;

    synchronized_buffer_.push(synchronized_packet);
    return SyncResult::OK;
}

bool TimeSynchronizer::interpIMU(uint64_t target_ts, ImuMeasurement &out) {
    constexpr size_t MAX_IMU_SAMPLES = 100;
    constexpr uint64_t MAX_TIME_SKEW = 50'000'000;  // 50 ms'

    size_t samples_read = 0;
    while (samples_read < MAX_IMU_SAMPLES) {
        ImuMeasurement measurement = imu_buffer_.pop();
        samples_read++;

        {
            std::lock_guard<std::mutex> lock(imu_mtx_);
            imu_window_.push_back(measurement);

            while (imu_window_.size() > MAX_IMU_SAMPLES) {
                imu_window_.pop_front();
            }

            if (imu_window_.size() < 2) {
                continue;
            }

            const auto prev = imu_window_[imu_window_.size() - 2];
            const auto next = imu_window_[imu_window_.size() - 1];

            if (next.timestamp_ns < target_ts &&
                (target_ts - MAX_TIME_SKEW) > next.timestamp_ns) {
                return false;
            }

            if (prev.timestamp_ns <= target_ts &&
                next.timestamp_ns >= target_ts) {
                // Interpolate
                double denom =
                static_cast<double>(next.timestamp_ns - prev.timestamp_ns);
                if (denom <= 0.0)
                    return false;  // non-monotonic timestamps

                double alpha =
                    static_cast<double>(target_ts - prev.timestamp_ns) / denom;

                out.timestamp_ns = target_ts;

                out.gyro_x = prev.gyro_x + alpha * (next.gyro_x - prev.gyro_x);
                out.gyro_y = prev.gyro_y + alpha * (next.gyro_y - prev.gyro_y);
                out.gyro_z = prev.gyro_z + alpha * (next.gyro_z - prev.gyro_z);

                out.accel_x = prev.accel_x + alpha * (next.accel_x - prev.accel_x);
                out.accel_y = prev.accel_y + alpha * (next.accel_y - prev.accel_y);
                out.accel_z = prev.accel_z + alpha * (next.accel_z - prev.accel_z);

                return true;
            }
        }
    }
    return false;
}

