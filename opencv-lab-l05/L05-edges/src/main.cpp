
#include "args.hpp"
#include "filters.hpp"
#include "convolution.hpp"
#include "utils.hpp"

#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv){
    ArgumentList args;
    if(!ParseInputs(args, argc, argv)) return 0;

    cv::Mat image = cv::imread(args.image_name, cv::IMREAD_GRAYSCALE);
    if(image.empty()){
        std::cerr << "Impossibile aprire: " << args.image_name << "\n";
        return 1;
    }
    showImage("original", image, args.wait_t, !args.noshow);

    // Gaussian
    cv::Mat blurred;
    {
        ScopedTimer T("Gaussian", args.timings);
        gaussianBlur(image, args.sigma, args.kernel_size/2, blurred, 1);
    }
    saveImage("out/gaussian", "gaussian.png", blurred, !args.nosave);
    showImage("gaussian", blurred, args.wait_t, !args.noshow);

    // Sobel
    cv::Mat magnitude, orientation;
    {
        ScopedTimer T("Sobel", args.timings);
        sobel3x3(blurred, magnitude, orientation);
    }
    // Visualization helpers
    cv::Mat mag8u, oriVis, oricolor;
    magnitude.convertTo(mag8u, CV_8UC1);
    orientation.copyTo(oriVis);
    for(int i=0;i<oriVis.rows*oriVis.cols;++i){
        if(mag8u.data[i] < 50) reinterpret_cast<float*>(oriVis.data)[i] = 0.f;
    }
    cv::convertScaleAbs(oriVis, oriVis, 255.0/(2*CV_PI));
    cv::applyColorMap(oriVis, oricolor, cv::COLORMAP_JET);
    saveImage("out/sobel", "magnitude.png", mag8u, !args.nosave);
    saveImage("out/sobel", "orientation.png", oricolor, !args.nosave);
    showImage("sobel magnitude", mag8u, args.wait_t, !args.noshow);
    showImage("sobel orientation", oricolor, args.wait_t, !args.noshow);

    // NMS
    cv::Mat nms, nms8u;
    {
        ScopedTimer T("NMS", args.timings);
        nonMaxSuppression(magnitude, orientation, nms);
    }
    nms.convertTo(nms8u, CV_8UC1);
    saveImage("out/nms", "edges_nms.png", nms8u, !args.nosave);
    showImage("edges after NMS", nms8u, args.wait_t, !args.noshow);

    // Double Threshold
    cv::Mat canny;
    {
        ScopedTimer T("DoubleThreshold", args.timings);
        if(doubleThreshold(nms, canny, args.low_th, args.high_th)){
            std::cerr << "Errore: low_th deve essere minore di high_th.\n";
            return 1;
        }
    }
    saveImage("out/canny", "edges_final.png", canny, !args.nosave);
    showImage("Canny final result", canny, args.wait_t, !args.noshow);

    return 0;
}
