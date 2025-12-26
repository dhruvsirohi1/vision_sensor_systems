//
// Created by Dhruv Sirohi on 12/25/25.
//

#ifndef CAMERA_DRIVER_HPP
#define CAMERA_DRIVER_HPP

#include <thread>

#include "camera_frame.hpp"
#include "ring_buffer.hpp"

class CameraDriver {

public:
    CameraDriver(int camera_id, int height, int width, int fps, RingBuffer<CameraFrame>& buffer);
    ~CameraDriver();

    void start();
    void stop();

    void captureLoop() const;

private:
    RingBuffer<CameraFrame>& buffer_;
    int camera_id_;
    int height_;
    int width_;
    std::atomic<bool> running_;
    std::thread worker_;
    int fps_;

};
#endif //CAMERA_DRIVER_HPP
