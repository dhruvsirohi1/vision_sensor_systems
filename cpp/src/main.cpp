//
// Created by Dhruv Sirohi on 12/25/25.
//

#include "camera_driver.hpp"
#include "ring_buffer.hpp"
#include<iostream>
#include "virtual_imu.hpp"

int main() {
    std::cout << "runnning..." << std::endl;
    RingBuffer<CameraFrame> camera_buffer(10);
    RingBuffer<ImuMeasurement> imu_buffer(100);

    CameraDriver camera(
    /*camera_id=*/0,
    /*height=*/640,
    /*width=*/480,
    /*fps=*/30,
    camera_buffer
    );

    VirtualImu virtual_imu(200.0, imu_buffer);

    camera.start();
    virtual_imu.start();

    for (int i = 0; i < 100; i++) {
        CameraFrame frame = camera_buffer.pop();
        ImuMeasurement imu_meas = imu_buffer.pop();

        std::cout << "Frame timestamp: " << frame.timestamp_ns << std::endl;
        std::cout << " | IMU ts: " << imu_meas.timestamp_ns
                  << " | gyro_z: " << imu_meas.gyro_z
                  << std::endl;

        cv::imshow("Camera", frame.frame);
        if (cv::waitKey(30) == 27) {break;}
    }

    camera.stop();
    return 0;
}
