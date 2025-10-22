
#include "io.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <filesystem>
#include <cstdio>

namespace fs = std::filesystem;

cv::Mat loadGray(const std::string& path){
    return cv::imread(path, cv::IMREAD_GRAYSCALE);
}

int show(const std::string& title, const cv::Mat& img, int waitMs, bool close){
    cv::namedWindow(title, cv::WINDOW_NORMAL);
    cv::imshow(title, img);
    int key = cv::waitKey(waitMs);
    if(close) cv::destroyWindow(title);
    return key;
}

std::string formatPattern(const std::string& pattern, int index){
    char buf[1024];
    std::snprintf(buf, sizeof(buf), pattern.c_str(), index);
    return std::string(buf);
}

void ensureDir(const std::string& path){
    if(path.empty()) return;
    fs::create_directories(path);
}

bool imwriteSafe(const std::string& path, const cv::Mat& img){
    try {
        fs::path p(path);
        if(!p.parent_path().empty()) fs::create_directories(p.parent_path());
        return cv::imwrite(path, img);
    } catch(...) { return false; }
}
