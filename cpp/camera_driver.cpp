//
// Created by Dhruv Sirohi on 12/25/25.
//

#pragma once
#include<opencv2/opencv.hpp>
#include "camera_driver.hpp"

using Clock = std::chrono::steady_clock;

CameraDriver::CameraDriver(int camera_id, int height, int width, int fps, RingBuffer<CameraFrame>& buffer) :
camera_id_(camera_id),
height_(height),
width_(width),
fps_(fps),
buffer_(buffer) {}

CameraDriver::~CameraDriver() {
    stop();
}

void CameraDriver::start() {
    running_ = true;
    worker_ = std::thread(&CameraDriver::captureLoop, this);
}

void CameraDriver::stop() {
    running_ = false;
    if (worker_.joinable()) {
        worker_.join();
    }
}

void CameraDriver::captureLoop() {
    cv::VideoCapture cap(camera_id_);

    if (!cap.isOpened()) {
        std::cerr << "Failed to open camera " << camera_id_ << std::endl;
        return;
    }

    cap.set(cv::CAP_PROP_FPS, fps_);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width_);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height_);

    const auto frame_period = std::chrono::nanoseconds(static_cast<int64_t>(1e9 / fps_));

    while (running_) {
        auto start = Clock::now;
        cv::Mat frame;
        if (!cap.read(frame)) {
            std::cerr << "Failed to read capture frame" << std::endl;
            continue;
        }
        uint64_t timestamp_ns = std::chrono::duration_cast<std::chrono::nanoseconds>
        (Clock::now().time_since_epoch()).count();

        CameraFrame camera_frame;
        camera_frame.timestamp_ns = timestamp_ns;
        camera_frame.frame = frame.clone();

        buffer_.push(camera_frame);
        std::this_thread::sleep_for(start + frame_period);
    }
    cap.release();
}