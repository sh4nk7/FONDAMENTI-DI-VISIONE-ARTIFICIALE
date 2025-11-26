// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// std
#include <iostream>
#include <limits>
#include <sys/stat.h>   // mkdir
#include <sys/types.h>  // mkdir

// --- PROTOTIPI ---
void computeDisparitySAD7x7(const cv::Mat &left, const cv::Mat &right, cv::Mat &disp);
void buildVDisparity(const cv::Mat &disp, cv::Mat &out, int max_d = 128);

int main(int argc, char **argv)
{
    if(argc != 3) {
        std::cerr << "Usage: ./stereo <left_image> <right_image>\n";
        return 1;
    }

    cv::Mat left  = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    cv::Mat right = cv::imread(argv[2], cv::IMREAD_GRAYSCALE);

    if(left.empty() || right.empty()) {
        std::cerr << "❌ Error loading input images\n";
        return 1;
    }

    cv::Mat disp32f;
    computeDisparitySAD7x7(left, right, disp32f);

    cv::Mat vdisp16u;
    buildVDisparity(disp32f, vdisp16u);

    // normalize for visualization
    cv::Mat normDisp, normVDisp;
    double minV = 0, maxV = 0;

    cv::minMaxLoc(disp32f, &minV, &maxV);
    cv::convertScaleAbs(disp32f, normDisp, 255.0 / (maxV - minV));

    cv::minMaxLoc(vdisp16u, &minV, &maxV);
    cv::convertScaleAbs(vdisp16u, normVDisp, 255.0 / (maxV - minV));

    cv::imshow("Left", left);
    cv::imshow("Right", right);
    cv::imshow("Disparity SAD", normDisp);
    cv::imshow("V-Disparity", normVDisp);

    // --- SAVE RESULTS (GENERIC PATH) ---
    std::string outDir = "out";
    mkdir(outDir.c_str(), 0777);

    cv::imwrite(outDir + "/disparity.png", normDisp);
    cv::imwrite(outDir + "/vdisparity.png", normVDisp);

    std::cout << "\n✅ Output saved in:\n";
    std::cout << "   → " << outDir << "/disparity.png\n";
    std::cout << "   → " << outDir << "/vdisparity.png\n\n";

    cv::waitKey();
    return 0;
}

void computeDisparitySAD7x7(const cv::Mat &left, const cv::Mat &right, cv::Mat &disp)
{
    const int win = 3; // 7x7 window radius
    disp = cv::Mat(left.size(), CV_32FC1, cv::Scalar(0));

    for(int r = win; r < left.rows - win; ++r)
    {
        for(int c = win; c < left.cols - win; ++c)
        {
            uint32_t bestCost = std::numeric_limits<uint32_t>::max();
            float bestD = 0;

            for(int d = 0; d < 128 && (c - d) >= win; ++d)
            {
                uint32_t cost = 0;

                for(int y = -win; y <= win; ++y)
                {
                    const uchar* lp = left.ptr<uchar>(r + y);
                    const uchar* rp = right.ptr<uchar>(r + y);

                    for(int x = -win; x <= win; ++x)
                        cost += std::abs(lp[c + x] - rp[(c + x) - d]);
                }

                if(cost < bestCost)
                {
                    bestCost = cost;
                    bestD = d;
                }
            }

            disp.at<float>(r, c) = bestD;
        }
    }
}

void buildVDisparity(const cv::Mat &disp, cv::Mat &out, int max_d)
{
    out = cv::Mat(disp.rows, max_d, CV_16UC1, cv::Scalar(0));

    for(int r = 0; r < disp.rows; ++r)
    {
        const float* row = disp.ptr<float>(r);

        for(int c = 0; c < disp.cols; ++c)
        {
            int d = static_cast<int>(row[c]);
            if(d > 0 && d < max_d)
                out.at<uint16_t>(r, d)++;
        }
    }
}

