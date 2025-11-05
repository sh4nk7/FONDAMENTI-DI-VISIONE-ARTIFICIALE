// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d.hpp>

struct CameraParams
{
    int w, h;          // dimensioni immagine
    float ku, kv;      // parametri intrinseci
    float u0, v0;      // centro principale
    cv::Affine3f RT;   // trasformazione extrinseca
};

void PoseToAffine(float rx, float ry, float rz, float tx, float ty, float tz, cv::Affine3f& affine)
{
    cv::Mat world_RvecX_cam = (cv::Mat_<float>(1,3) << rx, 0.0, 0.0);
    cv::Mat world_Rx_cam; cv::Rodrigues(world_RvecX_cam, world_Rx_cam);

    cv::Mat world_RvecY_cam = (cv::Mat_<float>(1,3) << 0.0, ry, 0.0);
    cv::Mat world_Ry_cam; cv::Rodrigues(world_RvecY_cam, world_Ry_cam);

    cv::Mat world_RvecZ_cam = (cv::Mat_<float>(1,3) << 0.0, 0.0, rz);
    cv::Mat world_Rz_cam; cv::Rodrigues(world_RvecZ_cam, world_Rz_cam);

    cv::Mat world_R_cam = world_Rx_cam * world_Ry_cam * world_Rz_cam;
    cv::Mat world_t_cam = (cv::Mat_<float>(1,3) << tx, ty, tz);
    affine = cv::Affine3f(world_R_cam, world_t_cam);
}

void LoadCameraParams(const std::string& filename, CameraParams& params)
{
    std::ifstream file(filename.c_str());
    file >> params.w >> params.h;
    file >> params.ku >> params.kv;
    file >> params.u0 >> params.v0;

    float rx, ry, rz, tx, ty, tz;
    file >> rx >> ry >> rz;
    file >> tx >> ty >> tz;

    PoseToAffine(rx, ry, rz, tx, ty, tz, params.RT);
}

