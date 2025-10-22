#include "utils.hpp"
#include <iostream>
#include <unistd.h>

bool ParseInputs(ArgumentList& args, int argc, char **argv) {
    int c;
    args.wait_t = 0;

    while ((c = getopt(argc, argv, "hi:t:")) != -1) {
        switch (c) {
            case 't': args.wait_t = atoi(optarg); break;
            case 'i': args.image_name = optarg; break;
            case 'h':
            default:
                std::cout << "usage: " << argv[0] << " -i <image_name>" << std::endl;
                std::cout << "exit:  type q\n\nAllowed options:\n"
                          << "   -h                       produce help message\n"
                          << "   -i arg                   image name. Use %0xd format for multiple images.\n"
                          << "   -t arg                   wait before next frame (ms)\n\n";
                return false;
        }
    }
    return true;
}

void addPadding(const cv::Mat image, cv::Mat &out, int vPadding, int hPadding) {
    out = cv::Mat(image.rows + vPadding * 2, image.cols + hPadding * 2, image.type(), cv::Scalar(0));
    for (int row = vPadding; row < out.rows - vPadding; ++row)
        for (int col = hPadding; col < out.cols - hPadding; ++col)
            for (int k = 0; k < out.channels(); ++k)
                out.data[((row * out.cols + col) * out.elemSize() + k * out.elemSize1())] =
                    image.data[(((row - vPadding) * image.cols + col - hPadding) * image.elemSize() + k * image.elemSize1())];
}

