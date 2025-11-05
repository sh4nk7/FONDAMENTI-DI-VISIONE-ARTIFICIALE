
#pragma once
#include <opencv2/opencv.hpp>

void gaussianKernel(float sigma, int radius, cv::Mat& kernel);
void gaussianBlur(const cv::Mat& src, float sigma, int radius, cv::Mat& dst, int stride=1);
void sobel3x3(const cv::Mat& src, cv::Mat& magnitude, cv::Mat& orientation);

// interpolation on CV_32FC1 matrix
float bilinear(const cv::Mat& src, float r, float c);

// Non-Maximum Suppression (NMS) along the gradient direction
void nonMaxSuppression(const cv::Mat& magnitude, const cv::Mat& orientation, cv::Mat& out);

// Double threshold (hysteresis-like). Returns 0 on success, 1 if thresholds are invalid.
int doubleThreshold(const cv::Mat& magn, cv::Mat& out, float t1, float t2);
