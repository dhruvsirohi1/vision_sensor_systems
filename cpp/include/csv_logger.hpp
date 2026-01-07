//
// Created by Dhruv Sirohi on 1/6/26.
//

#ifndef CSV_LOGGER_HPP
#define CSV_LOGGER_HPP
#include <fstream>
#include <mutex>
#include <string>

class CsvLogger {
public:
    explicit CsvLogger(const std::string& file_path);
    ~CsvLogger();

    void log(uint64_t cam_ts,
             uint64_t imu_ts,
             bool sync_success,
             int64_t imu_age_ns,
             int64_t latency_ns);

private:
    std::ofstream file_;
    std::mutex mtx_;
};
#endif //CSV_LOGGER_HPP
