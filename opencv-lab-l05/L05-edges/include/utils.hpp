
#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <chrono>

// Simple scoped timer
class ScopedTimer {
public:
    ScopedTimer(const std::string& label, bool enabled=true);
    ~ScopedTimer();
private:
    std::string label_;
    bool enabled_;
    std::chrono::high_resolution_clock::time_point t0_;
};

void showImage(const std::string& name, const cv::Mat& img, int wait=0, bool enabled=true);
void saveImage(const std::string& folder, const std::string& name, const cv::Mat& img, bool enabled=true);
