
#pragma once
#include <opencv2/core.hpp>
#include <string>

cv::Mat loadGray(const std::string& path);
int show(const std::string& title, const cv::Mat& img, int waitMs=1, bool close=false);
std::string formatPattern(const std::string& pattern, int index);
void ensureDir(const std::string& path);
bool imwriteSafe(const std::string& path, const cv::Mat& img);
