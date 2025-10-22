
#include "io.hpp"
#include "ops.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

cv::Mat loadImageSmart(const std::string& path) {
    // If filename hints a Bayer pattern, load as grayscale
    if (hasBayerTag(path))
        return cv::imread(path, cv::IMREAD_GRAYSCALE);
    return cv::imread(path, cv::IMREAD_COLOR);
}

int showAndWait(const std::string& title, const cv::Mat& img, int waitMs, bool autoclose) {
    cv::namedWindow(title, cv::WINDOW_AUTOSIZE);
    cv::imshow(title, img);
    int key = cv::waitKey(waitMs);
    if (autoclose) cv::destroyWindow(title);
    return key;
}
