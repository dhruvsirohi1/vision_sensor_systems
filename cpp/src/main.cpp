//
// Created by Dhruv Sirohi on 12/25/25.
//

#include<iostream>

#include "camera_driver.hpp"
#include "ring_buffer.hpp"
#include "simulator.hpp"
#include "virtual_imu.hpp"

int main() {
    std::cout << "runnning..." << std::endl;
    RingBuffer<CameraFrame> camera_buffer(10);
    RingBuffer<ImuMeasurement> imu_buffer(100);

    bool run_cam = false;
    bool run_vehicle = true;

    if (run_cam) {
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
    }
    // Test Simulator

    VehicleModel model;
    Simulator simulator(0.01, model);

    State x0{};
    x0.x = 0.0;
    x0.y = 0.0;
    x0.theta = 0.0;
    x0.v = 0.1;

    simulator.reset(x0);

    Control u{};
    u.a = 0.0;
    u.omega = 0.0;

    for (int i = 0; i < 100; i++) {
        simulator.step(u);

        auto curr_state = simulator.currentState();
        std::cout << curr_state.x << ", " << curr_state.y
        << ", " << curr_state.theta << std::endl;
    }
    return 0;
}
