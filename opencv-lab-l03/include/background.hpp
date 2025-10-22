
#pragma once
#include <opencv2/core.hpp>
#include <vector>

cv::Mat absDiffThresh(const cv::Mat& A, const cv::Mat& B, int thresh);
cv::Mat bgPrevFrame(const cv::Mat& frame, cv::Mat& bg, int thresh);
cv::Mat bgRunningMean(const cv::Mat& frame, std::vector<cv::Mat>& history, int window, int thresh, cv::Mat& bgOut);
cv::Mat bgEMA(const cv::Mat& frame, cv::Mat& bg, double alpha, int thresh);
