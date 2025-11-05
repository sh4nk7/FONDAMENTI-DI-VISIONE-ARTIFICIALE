
#include "convolution.hpp"
#include <iostream>

void addPadding(const cv::Mat& src, cv::Mat& dst, int vPad, int hPad){
    dst = cv::Mat(src.rows + 2*vPad, src.cols + 2*hPad, src.type(), cv::Scalar(0));
    src.copyTo(dst(cv::Rect(hPad, vPad, src.cols, src.rows)));
}

void myfilter2D(const cv::Mat& src, const cv::Mat& kernel, cv::Mat& dst, int stridev, int strideh){
    if(kernel.rows % 2 == 0 || kernel.cols % 2 == 0){
        std::cerr << "Kernel con dimensioni pari.\n";
        std::exit(1);
    }
    // output size identical to src when using zero padding + stride 1
    int outRows = (src.rows + (kernel.rows/2)*2 - kernel.rows)/stridev + 1;
    int outCols = (src.cols + (kernel.cols/2)*2 - kernel.cols)/strideh + 1;
    dst = cv::Mat(outRows, outCols, CV_32SC1, cv::Scalar(0));

    cv::Mat pad;
    addPadding(src, pad, kernel.rows/2, kernel.cols/2);

    int yc = kernel.rows/2;
    int xc = kernel.cols/2;
    const float* kdata = reinterpret_cast<const float*>(kernel.data);

    for(int i=0;i<dst.rows;++i){
        for(int j=0;j<dst.cols;++j){
            int oy = i*stridev + yc;
            int ox = j*strideh + xc;
            float acc = 0.f;
            for(int ky=-yc; ky<=yc; ++ky){
                for(int kx=-xc; kx<=xc; ++kx){
                    unsigned char pix = pad.data[(oy+ky)*pad.cols + (ox+kx)];
                    float kval = kdata[(ky+yc)*kernel.cols + (kx+xc)];
                    acc += pix * kval;
                }
            }
            reinterpret_cast<int*>(dst.data)[i*dst.cols + j] = static_cast<int>(acc);
        }
    }
}
