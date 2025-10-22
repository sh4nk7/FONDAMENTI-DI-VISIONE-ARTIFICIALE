#pragma once
#include <string>
#include <opencv2/opencv.hpp>

struct ArgumentList {
    std::string image_name;
    int wait_t;
};

bool ParseInputs(ArgumentList& args, int argc, char **argv);
void addPadding(const cv::Mat image, cv::Mat &out, int vPadding, int hPadding);

