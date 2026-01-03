//
// Created by Dhruv Sirohi on 1/2/26.
//

#ifndef SYNCHRONIZED_PACKET_HPP
#define SYNCHRONIZED_PACKET_HPP

#include "camera_frame.hpp"
#include "imu_measurement.hpp"

struct SynchronizedPacket {
    int64_t timestamp_ns;

    CameraFrame camera;
    ImuMeasurement imu;
};
#endif //SYNCHRONIZED_PACKET_HPP
