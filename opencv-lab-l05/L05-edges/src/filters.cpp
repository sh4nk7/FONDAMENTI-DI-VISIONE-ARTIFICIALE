
#include "filters.hpp"
#include "convolution.hpp"
#include <cmath>
#include <iostream>

void gaussianKernel(float sigma, int radius, cv::Mat& kernel){
    kernel = cv::Mat(2*radius+1, 1, CV_32FC1);
    float sum = 0.f;
    for(int i=-radius;i<=radius;++i){
        float v = std::exp(-(i*i)/(2.f*sigma*sigma));
        kernel.at<float>(i+radius,0) = v;
        sum += v;
    }
    kernel /= sum;
}

void gaussianBlur(const cv::Mat& src, float sigma, int radius, cv::Mat& dst, int stride){
    cv::Mat vg; gaussianKernel(sigma, radius, vg);
    cv::Mat tmp;
    myfilter2D(src, vg.t(), tmp, 1, stride);
    tmp.convertTo(tmp, CV_8UC1);
    myfilter2D(tmp, vg, dst, stride, 1);
    dst.convertTo(dst, CV_8UC1);
}

void sobel3x3(const cv::Mat& src, cv::Mat& magnitude, cv::Mat& orientation){
    cv::Mat hx = (cv::Mat_<float>(3,3) << -1,0,1,-2,0,2,-1,0,1);
    cv::Mat hy = hx.t();
    cv::Mat gx, gy;
    myfilter2D(src, hx, gx);
    myfilter2D(src, hy, gy);
    gx.convertTo(gx, CV_32FC1);
    gy.convertTo(gy, CV_32FC1);

    magnitude.create(src.size(), CV_32FC1);
    orientation.create(src.size(), CV_32FC1);
    for(int r=0;r<src.rows;++r){
        const float* px = gx.ptr<float>(r);
        const float* py = gy.ptr<float>(r);
        float* pm = magnitude.ptr<float>(r);
        float* po = orientation.ptr<float>(r);
        for(int c=0;c<src.cols;++c){
            float x = px[c], y = py[c];
            pm[c] = std::sqrt(x*x + y*y);
            po[c] = std::atan2(y, x);
        }
    }
}

float bilinear(const cv::Mat& src, float r, float c){
    int y = (int)r; int x = (int)c;
    float dy = r - y; float dx = c - x;
    // boundary guard (simple clamp inside valid range)
    y = std::max(0, std::min(y, src.rows-2));
    x = std::max(0, std::min(x, src.cols-2));
    const float* p00 = src.ptr<float>(y) + x;
    const float* p10 = src.ptr<float>(y+1) + x;
    float v = p00[0]*(1-dy)*(1-dx)
            + p10[0]*dy*(1-dx)
            + p00[1]*(1-dy)*dx
            + p10[1]*dy*dx;
    return v;
}

void nonMaxSuppression(const cv::Mat& magnitude, const cv::Mat& orientation, cv::Mat& out){
    out = cv::Mat::zeros(magnitude.size(), CV_32FC1);
    for(int r=1;r<magnitude.rows-1;++r){
        for(int c=1;c<magnitude.cols-1;++c){
            float theta = orientation.at<float>(r,c);
            float e1 = bilinear(magnitude, r + std::sin(theta), c + std::cos(theta));
            float e2 = bilinear(magnitude, r - std::sin(theta), c - std::cos(theta));
            float p = magnitude.at<float>(r,c);
            if(p >= e1 && p >= e2) out.at<float>(r,c) = p;
        }
    }
}

int doubleThreshold(const cv::Mat& magn, cv::Mat& out, float t1, float t2){
    if(t1 >= t2) return 1;
    out = cv::Mat::zeros(magn.size(), CV_8UC1);
    // weak value between thresholds
    const unsigned char WEAK = 128;
    for(int r=0;r<magn.rows;++r){
        const float* pm = magn.ptr<float>(r);
        unsigned char* po = out.ptr<unsigned char>(r);
        for(int c=0;c<magn.cols;++c){
            float v = pm[c];
            if(v >= t2) po[c] = 255;
            else if(v > t1) po[c] = WEAK;
        }
    }
    // simple connectivity: promote weak with a strong 8-neighbour
    for(int r=1;r<out.rows-1;++r){
        for(int c=1;c<out.cols-1;++c){
            if(out.at<unsigned char>(r,c) == 128){
                bool neigh=false;
                for(int dr=-1;dr<=1 && !neigh;++dr)
                    for(int dc=-1;dc<=1 && !neigh;++dc)
                        if(out.at<unsigned char>(r+dr,c+dc)==255) neigh=true;
                out.at<unsigned char>(r,c) = neigh ? 255 : 0;
            }
        }
    }
    return 0;
}
