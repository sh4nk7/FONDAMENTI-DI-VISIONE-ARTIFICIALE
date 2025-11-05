
#pragma once
#include <opencv2/opencv.hpp>

void addPadding(const cv::Mat& src, cv::Mat& dst, int vPad, int hPad);

// src: CV_8UC1, kernel: CV_32FC1, dst: CV_32SC1
// stridev/strideh default 1
void myfilter2D(const cv::Mat& src, const cv::Mat& kernel, cv::Mat& dst, int stridev=1, int strideh=1);
