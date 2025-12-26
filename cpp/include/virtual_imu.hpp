//
// Created by Dhruv Sirohi on 12/25/25.
//

#ifndef VIRTUAL_IMU_HPP
#define VIRTUAL_IMU_HPP

#include <random>
#include <thread>

#include "imu_measurement.hpp"
#include "ring_buffer.hpp"

class VirtualImu {
public:
    VirtualImu(double rate_hz, RingBuffer<ImuMeasurement>& buffer);
    ~VirtualImu();

    void start();
    void stop();

private:

    void imuLoop();
    double rate_hz_;
    RingBuffer<ImuMeasurement>& buffer_;

    std::atomic<bool> running_;
    std::thread worker_;

    std::default_random_engine rng_;
    std::normal_distribution<double> normal_distribution_;

    double gyro_bias[3];
    double accel_bias[3];
};
#endif //VIRTUAL_IMU_HPP
