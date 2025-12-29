//
// Created by Dhruv Sirohi on 12/25/25.
//

#include "virtual_imu.hpp"
#include <chrono>
#include <cmath>

using Clock = std::chrono::steady_clock;

VirtualImu::VirtualImu(double rate_hz, RingBuffer<ImuMeasurement> &buffer) :
rate_hz_(rate_hz),
buffer_(buffer),
running_(false),
noise_(0.0, 0.1) {
    gyro_bias_[0] = 0.005;
    gyro_bias_[1] = -0.003;
    gyro_bias_[2] = 0.002;

    accel_bias_[0] = 0.1;
    accel_bias_[1] = -0.05;
    accel_bias_[2] = 0.2;
}

VirtualImu::~VirtualImu() {
    stop();
}

void VirtualImu::start() {
    running_ = true;
    worker_ = std::thread(&VirtualImu::imuLoop, this);
}

void VirtualImu::stop() {
    running_ = false;
    if (worker_.joinable()) {
        worker_.join();
    }
}

void VirtualImu::imuLoop() {
    const auto period =
        std::chrono::nanoseconds(static_cast<int64_t>(1e9 / rate_hz_));

    double t = 0.0;

    while (running_) {
        auto start = Clock::now();

        ImuMeasurement imu;
        imu.timestamp_ns =
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                Clock::now().time_since_epoch()).count();

        // Simulated angular velocity (slow rotation)
        imu.gyro_x = 0.1 * std::sin(t) + gyro_bias_[0] + noise_(rng_);
        imu.gyro_y = 0.1 * std::cos(t) + gyro_bias_[1] + noise_(rng_);
        imu.gyro_z = 0.05 + gyro_bias_[2] + noise_(rng_);

        // Simulated acceleration (gravity + motion)
        imu.accel_x = accel_bias_[0] + noise_(rng_);
        imu.accel_y = accel_bias_[1] + noise_(rng_);
        imu.accel_z = 9.81 + accel_bias_[2] + noise_(rng_);

        buffer_.push(imu);

        t += 1.0 / rate_hz_;
        std::this_thread::sleep_until(start + period);
    }
}
