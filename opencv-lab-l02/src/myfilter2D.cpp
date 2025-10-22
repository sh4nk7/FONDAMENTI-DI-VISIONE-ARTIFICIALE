#include "myfilter2D.hpp"
#include "utils.hpp"
#include <iostream>

void myfilter2D(const cv::Mat& src, const cv::Mat& krn, cv::Mat& out, int stride) {
    if (krn.rows % 2 == 0 || krn.cols % 2 == 0) {
        std::cerr << "myfilter2D(): ERROR — kernel has not odd size!" << std::endl;
        exit(1);
    }

    int outsizey = (src.rows + (krn.rows / 2) * 2 - krn.rows) / (float)stride + 1;
    int outsizex = (src.cols + (krn.cols / 2) * 2 - krn.cols) / (float)stride + 1;
    out = cv::Mat(outsizey, outsizex, CV_32SC1);

    cv::Mat image;
    addPadding(src, image, krn.rows / 2, krn.cols / 2);

    int xc = krn.cols / 2;
    int yc = krn.rows / 2;

    int *outbuffer = reinterpret_cast<int*>(out.data);
    const float *kernel = reinterpret_cast<const float*>(krn.data);

    for (int i = 0; i < out.rows; ++i) {
        for (int j = 0; j < out.cols; ++j) {
            int origy = i * stride + yc;
            int origx = j * stride + xc;
            float sum = 0;
            for (int ki = -yc; ki <= yc; ++ki)
                for (int kj = -xc; kj <= xc; ++kj)
                    sum += image.data[(origy + ki) * image.cols + (origx + kj)] *
                           kernel[(ki + yc) * krn.cols + (kj + xc)];
            outbuffer[i * out.cols + j] = static_cast<int>(sum);
        }
    }
}

