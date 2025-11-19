#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d.hpp>
#include <fstream>
#include <stdexcept>

struct CameraParams
{
    int w, h;          
    float ku, kv;      
    float u0, v0;      
    cv::Affine3f RT;   
};

// ------------------------------------------------------------
// Converte rotazioni (rx, ry, rz) e traslazioni in Affine3f
// ------------------------------------------------------------
void PoseToAffine(float rx, float ry, float rz,
                  float tx, float ty, float tz,
                  cv::Affine3f& affine)
{
    cv::Mat RvecX = (cv::Mat_<float>(1,3) << rx, 0.0f, 0.0f);
    cv::Mat RvecY = (cv::Mat_<float>(1,3) << 0.0f, ry, 0.0f);
    cv::Mat RvecZ = (cv::Mat_<float>(1,3) << 0.0f, 0.0f, rz);

    cv::Mat Rx, Ry, Rz;
    cv::Rodrigues(RvecX, Rx);
    cv::Rodrigues(RvecY, Ry);
    cv::Rodrigues(RvecZ, Rz);

    cv::Mat R = Rx * Ry * Rz;
    cv::Mat t = (cv::Mat_<float>(3,1) << tx, ty, tz);

    affine = cv::Affine3f(R, t);
}

// ------------------------------------------------------------
// Caricamento dei parametri da file .txt
// ------------------------------------------------------------
void LoadCameraParams(const std::string& filename, CameraParams& params)
{
    std::ifstream f(filename);
    if(!f.is_open())
        throw std::runtime_error("Cannot open camera params file!");

    float rx, ry, rz, tx, ty, tz;

    f >> params.w >> params.h;
    f >> params.ku >> params.kv;
    f >> params.u0 >> params.v0;
    f >> rx >> ry >> rz;
    f >> tx >> ty >> tz;

    PoseToAffine(rx, ry, rz, tx, ty, tz, params.RT);
}

