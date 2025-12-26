//
// Created by Dhruv Sirohi on 12/25/25.
//

#include "camera_driver.hpp"
#include "ring_buffer.hpp"
#include<iostream>

int main() {
    std::cout << "runnning..." << std::endl;
    RingBuffer<CameraFrame> camera_buffer(10);

    CameraDriver camera(
    /*camera_id=*/0,
    /*height=*/640,
    /*width=*/480,
    /*fps=*/30,
    camera_buffer
    );

    camera.start();

    for (int i = 0; i < 100; i++) {
        CameraFrame frame = camera_buffer.pop();
        std::cout << "Frame timestamp: " << frame.timestamp_ns << std::endl;
        cv::imshow("Camera", frame.frame);
        if (cv::waitKey(30) == 27) {break;}
    }

    camera.stop();
    return 0;
}