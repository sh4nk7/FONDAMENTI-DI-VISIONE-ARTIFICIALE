// std
#include <iostream>
#include <fstream>

// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui.hpp>

// eigen
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

// utils
#include "utils.h"

using namespace std;
using namespace Eigen;

int main(int argc, char **argv) {

    if (argc < 3) {
        cerr << "Usage: lab5_2 <image_filename> <camera_params_filename>" << endl;
        return 0;
    }

    // output image
    cv::Mat ipm_image(cv::Size(400, 400), CV_8UC3, cv::Scalar(0, 0, 0));

    // load image
    cv::Mat input = cv::imread(argv[1]);
    if (input.empty()) {
        cerr << "Error loading image." << endl;
        return 1;
    }

    cv::imshow("Input", input);
    cv::waitKey(10);

    // load camera parameters
    CameraParams params;
    LoadCameraParams(argv[2], params);

    // build K matrix
    Eigen::Matrix<float, 3, 4> K;
    K << params.ku, 0.0, params.u0, 0.0,
         0.0, params.kv, params.v0, 0.0,
         0.0, 0.0, 1.0, 0.0;

    // build RT matrix
    Eigen::Matrix<float, 4, 4> RT;
    cv::Affine3f RT_inv = params.RT.inv();
    RT << RT_inv.matrix(0,0), RT_inv.matrix(0,1), RT_inv.matrix(0,2), RT_inv.matrix(0,3),
          RT_inv.matrix(1,0), RT_inv.matrix(1,1), RT_inv.matrix(1,2), RT_inv.matrix(1,3),
          RT_inv.matrix(2,0), RT_inv.matrix(2,1), RT_inv.matrix(2,2), RT_inv.matrix(2,3),
          0, 0, 0, 1;

    // TODO: implementare la costruzione di M = K * RT
    // e l'applicazione dell'Inverse Perspective Mapping (piano Y=0)

    cv::imshow("IPM", ipm_image);
    cout << "Press any key to quit" << endl;
    cv::waitKey(0);

    return 0;
}

