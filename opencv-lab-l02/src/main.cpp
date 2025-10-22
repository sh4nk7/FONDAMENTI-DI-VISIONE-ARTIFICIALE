#include "myfilter2D.hpp"
#include "utils.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char **argv) {
    int frame_number = 0;
    char frame_name[256];
    int ksize = 3, stride = 1;
    bool exit_loop = false;

    ArgumentList args;
    if (!ParseInputs(args, argc, argv))
        return 0;

    while (!exit_loop) {
        if (args.image_name.find('%') != std::string::npos)
            sprintf(frame_name, args.image_name.c_str(), frame_number);
        else
            sprintf(frame_name, "%s", args.image_name.c_str());

        std::cout << "Opening " << frame_name << std::endl;
        cv::Mat image = cv::imread(frame_name);
        if (image.empty()) {
            std::cerr << "Unable to open " << frame_name << std::endl;
            return 1;
        }

        cv::imshow("original image", image);

        // Conversione in scala di grigi
        cv::Mat grey;
        cv::cvtColor(image, grey, cv::COLOR_BGR2GRAY);

        // BOX FILTER
        cv::Mat blurred, blurdisplay;
        cv::Mat custom_kernel(ksize, ksize, CV_32FC1, 1.0 / (ksize * ksize));
        myfilter2D(grey, custom_kernel, blurred, stride);
        cv::convertScaleAbs(blurred, blurdisplay);
        cv::imshow("Box filter (myfilter2D)", blurdisplay);

        cv::Mat blurred_ref;
        cv::boxFilter(grey, blurred_ref, CV_8U, cv::Size(ksize, ksize));
        cv::imshow("Box filter (OpenCV)", blurred_ref);

        // SOBEL
        cv::Mat h_sobel = (cv::Mat_<float>(3, 3) <<
                           -1, 0, 1,
                           -2, 0, 2,
                           -1, 0, 1);
        cv::Mat v_sobel = h_sobel.t();

        cv::Mat my_gx, my_gy;
        myfilter2D(grey, h_sobel, my_gx, stride);
        myfilter2D(grey, v_sobel, my_gy, stride);

        cv::Mat gx, gy, magnitude;
        my_gx.convertTo(gx, CV_32F);
        my_gy.convertTo(gy, CV_32F);
        cv::magnitude(gx, gy, magnitude);

        cv::imshow("Sobel X", cv::abs(gx));
        cv::imshow("Sobel Y", cv::abs(gy));
        cv::imshow("Sobel Magnitude", cv::abs(magnitude));

        unsigned char key = cv::waitKey(args.wait_t);
        if (key == 'q') exit(0);
        if (key == 'S') stride++;
        if (key == 's' && stride > 1) stride--;
        if (key == 'k') {
            static int idx = 0;
            int values[] = {3, 5, 7, 11, 13};
            ksize = values[++idx % 5];
            std::cout << "Kernel size = " << ksize << std::endl;
        }
        frame_number++;
    }

    return 0;
}

