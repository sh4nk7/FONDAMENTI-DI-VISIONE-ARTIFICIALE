// std
#include <iostream>
#include <fstream>
#include <string>

// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

// eigen
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

// utils
#include "utils.h"

// ------------------------------------------------------------
// Bilinear interpolation
// ------------------------------------------------------------
cv::Vec3b bilinear(const cv::Mat& src, float r, float c)
{
    float y = r - (int)r;
    float x = c - (int)c;

    cv::Vec3b v =
        src.at<cv::Vec3b>(r, c) * (1-y)*(1-x) +
        src.at<cv::Vec3b>(r+1, c) * y*(1-x) +
        src.at<cv::Vec3b>(r, c+1) * (1-y)*x +
        src.at<cv::Vec3b>(r+1, c+1) * y*x;

    return v;
}

int main(int argc, char **argv)
{
    if(argc < 3)
    {
        std::cerr << "Usage: lab5_2 <image_filename> <camera_params_filename>\n";
        return 0;
    }

    // ------------------------------
    // Load input image
    // ------------------------------
    cv::Mat input = cv::imread(argv[1]);
    if(input.empty())
    {
        std::cerr << "Error loading input image\n";
        return 1;
    }

    cv::imshow("Input", input);
    cv::waitKey(10);

    // ------------------------------
    // Load camera parameters
    // ------------------------------
    CameraParams params;
    LoadCameraParams(argv[2], params);

    // ------------------------------
    // Build K (3x4)
    // ------------------------------
    Eigen::Matrix<float,3,4> K;
    K << params.ku, 0, params.u0, 0,
         0, params.kv, params.v0, 0,
         0, 0, 1, 0;

    // ------------------------------
    // Build RT
    // ------------------------------
    cv::Affine3f RT_inv = params.RT.inv();
    Eigen::Matrix<float,4,4> RT;

    RT << RT_inv.matrix(0,0), RT_inv.matrix(0,1), RT_inv.matrix(0,2), RT_inv.matrix(0,3),
          RT_inv.matrix(1,0), RT_inv.matrix(1,1), RT_inv.matrix(1,2), RT_inv.matrix(1,3),
          RT_inv.matrix(2,0), RT_inv.matrix(2,1), RT_inv.matrix(2,2), RT_inv.matrix(2,3),
          0,0,0,1;

    // ------------------------------
    // M = K * RT
    // ------------------------------
    Eigen::Matrix<float,3,4> M = K * RT;

    // ------------------------------
    // Remove Y (piano Y=0)
    // ------------------------------
    Eigen::Matrix3f M_r;
    M_r << M(0,0), M(0,2), M(0,3),
           M(1,0), M(1,2), M(1,3),
           M(2,0), M(2,2), M(2,3);

    Eigen::Matrix3f IPM = M_r.inverse();

    // ------------------------------
    // Prepare output images
    // ------------------------------
    cv::Mat sparse_ipm(400,400,CV_8UC3,cv::Scalar(0,0,0));
    cv::Mat dense_ipm(400,400,CV_8UC3,cv::Scalar(0,0,0));
    cv::Mat bilinear_dense_ipm(400,400,CV_8UC3,cv::Scalar(0,0,0));

    double k = 50.0;

    // ------------------------------------------------------
    // Sparse IPM
    // ------------------------------------------------------
    for(int r=0; r<input.rows; r++)
    {
        for(int c=0; c<input.cols; c++)
        {
            Eigen::Vector3f proj = IPM * Eigen::Vector3f(c, r, 1);

            int X = proj.x()/proj.z()*k + sparse_ipm.cols/2.0;
            int Z = sparse_ipm.rows - proj.y()/proj.z()*k;

            if(X>=0 && X<sparse_ipm.cols && Z>=0 && Z<sparse_ipm.rows)
                sparse_ipm.at<cv::Vec3b>(Z,X) = input.at<cv::Vec3b>(r,c);
        }
    }

    // ------------------------------------------------------
    // Dense + Bilinear IPM
    // ------------------------------------------------------
    for(int r=0; r<dense_ipm.rows; r++)
    {
        for(int c=0; c<dense_ipm.cols; c++)
        {
            Eigen::Vector3f pt((c - dense_ipm.cols/2)/k, r/k, 1);
            Eigen::Vector3f uv = M_r * pt;

            float u = uv.x()/uv.z();
            float v = uv.y()/uv.z();

            if(u>=0 && u<input.cols-1 && v>=0 && v<input.rows-1)
            {
                dense_ipm.at<cv::Vec3b>(dense_ipm.rows-r-1, c) = input.at<cv::Vec3b>(v,u);
                bilinear_dense_ipm.at<cv::Vec3b>(dense_ipm.rows-r-1, c) = bilinear(input,v,u);
            }
        }
    }

    // ------------------------------------------------------
    // Show output
    // ------------------------------------------------------
    cv::imshow("Sparse IPM", sparse_ipm);
    cv::imshow("Dense IPM", dense_ipm);
    cv::imshow("Bilinear Dense IPM", bilinear_dense_ipm);

    // ------------------------------------------------------
    // SAVE OUTPUT USING INPUT NAME
    // gas.jpg → gas_out.png
    // ------------------------------------------------------
    std::string input_path = argv[1];
    std::string fname = input_path.substr(input_path.find_last_of("/\\") + 1);

    size_t dot = fname.find_last_of('.');
    std::string noext = fname.substr(0, dot);

    std::string out_path = "../out/" + noext + "_out.png";

    cv::imwrite(out_path, bilinear_dense_ipm);

    std::cout << "Salvato: " << out_path << std::endl;

    cv::waitKey(0);
    return 0;
}

