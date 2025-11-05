
#include "utils.hpp"
#include <filesystem>
#include <iostream>

ScopedTimer::ScopedTimer(const std::string& label, bool enabled)
: label_(label), enabled_(enabled), t0_(std::chrono::high_resolution_clock::now()) {}

ScopedTimer::~ScopedTimer(){
    if(!enabled_) return;
    auto t1 = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0_).count();
    std::cout << label_ << ": " << ms << " ms\n";
}

void showImage(const std::string& name, const cv::Mat& img, int wait, bool enabled){
    if(!enabled) return;
    cv::namedWindow(name, cv::WINDOW_NORMAL);
    cv::imshow(name, img);
    cv::waitKey(wait);
}

void saveImage(const std::string& folder, const std::string& name, const cv::Mat& img, bool enabled){
    if(!enabled) return;
    std::filesystem::create_directories(folder);
    std::string path = folder + "/" + name;
    cv::imwrite(path, img);
}
