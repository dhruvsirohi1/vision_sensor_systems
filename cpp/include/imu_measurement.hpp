//
// Created by Dhruv Sirohi on 12/25/25.
//

#ifndef IMU_MEASUREMENT_HPP
#define IMU_MEASUREMENT_HPP
#include <cstdint>

struct ImuMeasurement {
    uint64_t timestamp_ns;

    double gyro_x;
    double gyro_y;
    double gyro_z;

    double accel_x;
    double accel_y;
    double accel_z;
};
#endif //IMU_MEASUREMENT_HPP
