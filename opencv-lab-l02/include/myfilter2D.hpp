#pragma once
#include <opencv2/opencv.hpp>

void myfilter2D(
    const cv::Mat& src,
    const cv::Mat& krn,
    cv::Mat& out,
    int stride = 1
);

