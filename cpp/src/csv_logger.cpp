//
// Created by Dhruv Sirohi on 1/7/26.
//
#include "csv_logger.hpp"

CsvLogger::CsvLogger(const std::string &file_path) {
    file_.open(file_path, std::ios::out | std::ios::app);
    if (!file_.is_open()) {
        file_ << "camera_ts_ns,imu_ts_ns,sync_success,imu_age_ns,latency_ns\n";
    }
}

CsvLogger::~CsvLogger() {
    if (file_.is_open()) {
        file_.close();
    }
}

void CsvLogger::log(uint64_t cam_ts, uint64_t imu_ts, bool sync_success, int64_t imu_age_ns, int64_t latency_ns) {
    std::lock_guard<std::mutex> lock(mtx_);
    file_ << cam_ts << ","
          << imu_ts << ","
          << sync_success << ","
          << imu_age_ns << ","
          << latency_ns << "\n";
}
