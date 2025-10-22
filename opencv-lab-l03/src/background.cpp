
#include "background.hpp"
#include <opencv2/imgproc.hpp>
#include <algorithm>

cv::Mat absDiffThresh(const cv::Mat& A, const cv::Mat& B, int thresh){
    CV_Assert(A.type()==CV_8UC1 && B.type()==CV_8UC1 && A.size()==B.size());
    cv::Mat a16, b16, d16;
    A.convertTo(a16, CV_16SC1);
    B.convertTo(b16, CV_16SC1);
    cv::absdiff(a16, b16, d16);
    cv::threshold(d16, d16, thresh, 255, cv::THRESH_BINARY);
    cv::Mat out;
    d16.convertTo(out, CV_8UC1);
    return out;
}

cv::Mat bgPrevFrame(const cv::Mat& frame, cv::Mat& bg, int thresh){
    if(bg.empty()){ bg = frame.clone(); return cv::Mat::zeros(frame.size(), CV_8UC1); }
    cv::Mat fg = absDiffThresh(frame, bg, thresh);
    frame.copyTo(bg);
    return fg;
}

cv::Mat bgRunningMean(const cv::Mat& frame, std::vector<cv::Mat>& history, int window, int thresh, cv::Mat& bgOut){
    history.push_back(frame);
    if((int)history.size() > window) history.erase(history.begin());
    if((int)history.size() < window){
        bgOut = frame.clone();
        return cv::Mat::zeros(frame.size(), CV_8UC1);
    }
    cv::Mat acc(frame.size(), CV_32FC1, cv::Scalar(0));
    for(const auto& f : history) cv::accumulate(f, acc);
    acc /= (float)history.size();
    acc.convertTo(bgOut, CV_8UC1);
    return absDiffThresh(frame, bgOut, thresh);
}

cv::Mat bgEMA(const cv::Mat& frame, cv::Mat& bg, double alpha, int thresh){
    if(bg.empty()){ frame.copyTo(bg); return cv::Mat::zeros(frame.size(), CV_8UC1); }
    cv::Mat fg = absDiffThresh(frame, bg, thresh);
    cv::addWeighted(bg, alpha, frame, 1.0 - alpha, 0.0, bg);
    return fg;
}
