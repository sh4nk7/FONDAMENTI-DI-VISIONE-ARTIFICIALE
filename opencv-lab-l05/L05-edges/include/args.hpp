
#pragma once
#include <string>

struct ArgumentList {
    std::string image_name;  // path to grayscale image
    int wait_t = 0;          // wait time for imshow
    float sigma = 1.0f;      // gaussian sigma
    int kernel_size = 3;     // gaussian kernel size (odd)
    float low_th = 50.0f;    // low threshold for hysteresis
    float high_th = 150.0f;  // high threshold for hysteresis
    bool noshow = false;     // disable GUI windows
    bool nosave = false;     // disable saving outputs
    bool timings = true;     // print timings
};

bool ParseInputs(ArgumentList& args, int argc, char** argv);
