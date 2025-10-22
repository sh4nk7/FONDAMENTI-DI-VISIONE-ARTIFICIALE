
#pragma once
#include <opencv2/core.hpp>
#include <vector>
#include <string>

// Basic ops
cv::Mat downsample2x(const cv::Mat& src);
cv::Mat downsampleRows2x(const cv::Mat& src);
cv::Mat downsampleCols2x(const cv::Mat& src);
cv::Mat flipHorizontal(const cv::Mat& src);
cv::Mat flipVertical(const cv::Mat& src);
cv::Mat cropRegion(const cv::Mat& src, int u, int v, int w, int h);
cv::Mat addZeroPadding(const cv::Mat& src, int pad);
cv::Mat shuffleBlocks4(const cv::Mat& src);
cv::Mat shuffleChannels(const cv::Mat& src);

// Bayer helpers
bool hasBayerTag(const std::string& filename);
enum class Bayer { RGGB, GBRG, BGGR, NONE };

Bayer detectBayer(const std::string& filename);

// Debayer variants
cv::Mat debayerDownsampleG(const cv::Mat& bayer, Bayer pattern); // 8UC1 w/2 x h/2
cv::Mat debayerLuminance(const cv::Mat& bayer, Bayer pattern);   // 8UC1 w x h
cv::Mat debayerSimple(const cv::Mat& bayer, Bayer pattern);      // 8UC3 w x h
cv::Mat debayerSimpleDown(const cv::Mat& bayer, Bayer pattern);  // 8UC3 w/2 x h/2
