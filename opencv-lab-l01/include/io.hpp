
#pragma once
#include <opencv2/core.hpp>
#include <string>

// Load color image if not Bayer, or grayscale if filename suggests a Bayer pattern.
cv::Mat loadImageSmart(const std::string& path);

// Show (and optionally close) window, return pressed key (or -1 on timeout).
int showAndWait(const std::string& title, const cv::Mat& img, int waitMs = 0, bool autoclose = false);
