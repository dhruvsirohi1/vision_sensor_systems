//
// Created by Dhruv Sirohi on 12/25/25.
//

#ifndef CAMERA_FRAME_HPP
#define CAMERA_FRAME_HPP

#include<opencv2/opencv.hpp>
#include<cstdint>

struct CameraFrame {
    int64_t timestamp_ns;
    cv::Mat frame;
};
#endif //CAMERA_FRAME_HPP
