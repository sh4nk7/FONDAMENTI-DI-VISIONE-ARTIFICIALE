// -------------------------------------------------------------
//  LAB L08 - RANSAC LINE FITTING 
// -------------------------------------------------------------

//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//std:
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <vector>
#include <filesystem>

struct ArgumentList {
    std::string image_name;
    std::string points_name;
    int dist_t;
    int wait_t;
};

bool ParseInputs(ArgumentList& args, int argc, char **argv);


// ================================================================
// DISTANZA PUNTO–RETTA
// ================================================================
double Distance2Line(cv::Point A, cv::Point B, cv::Point P)
{
    double normalLength = hypot(B.x - A.x, B.y - A.y);
    return fabs((double)((P.x - A.x)*(B.y - A.y) - (P.y - A.y)*(B.x - A.x)) / normalLength);
}


// ================================================================
// LETTURA PUNTI — CORRETTA (file contiene: riga colonna)
// ================================================================
bool ReadPoints(const std::string& filename, std::vector<cv::Point>& pts)
{
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "Errore apertura file punti: " << filename << std::endl;
        return false;
    }

    pts.clear();
    int r, c; // row, column

    while (in >> r >> c) {
        pts.emplace_back(c, r);   // OpenCV usa (x = colonna, y = riga)
    }

    std::cout << "Letti " << pts.size() << " punti da " << filename << std::endl;
    return !pts.empty();
}


// ================================================================
// MAIN PROGRAM
// ================================================================
int main(int argc, char **argv)
{
    srand(time(0));

    ArgumentList args;
    if(!ParseInputs(args, argc, argv)) return 0;

    // crea cartella out/ se non esiste
    std::filesystem::create_directories("../out");

    int frame_number = 0;
    char frame_name[256];
    bool exit_loop = false;

    while(!exit_loop)
    {
        // -------------------------------------------------------------
        // COSTRUZIONE NOME IMMAGINE
        // -------------------------------------------------------------
        if(args.image_name.find('%') != std::string::npos)
            sprintf(frame_name, args.image_name.c_str(), frame_number);
        else
            sprintf(frame_name, "%s", args.image_name.c_str());

        std::cout << "Opening " << frame_name << std::endl;

        cv::Mat image = cv::imread(frame_name, cv::IMREAD_COLOR);
        if(image.empty()) {
            std::cerr << "Unable to open " << frame_name << std::endl;
            return 1;
        }

        cv::Mat out = image.clone();

        // -------------------------------------------------------------
        // LETTURA PUNTI
        // -------------------------------------------------------------
        std::vector<cv::Point> points;
        if (!ReadPoints(args.points_name, points))
            return 1;

        // -------------------------------------------------------------
        // RANSAC
        // -------------------------------------------------------------
        int best_inliers = -1;
        cv::Point best_A, best_B;

        const int ITER = 500;
        const double T = args.dist_t;

        for (int it = 0; it < ITER; it++)
        {
            int i1 = rand() % points.size();
            int i2 = rand() % points.size();
            if (i1 == i2) continue;

            cv::Point A = points[i1];
            cv::Point B = points[i2];

            int inl = 0;
            for (auto& p : points)
                if (Distance2Line(A, B, p) < T)
                    inl++;

            // Se questa soluzione è migliore, aggiorna
            if (inl > best_inliers)
            {
                best_inliers = inl;
                best_A = A;
                best_B = B;

                // disegno modello e salvo soluzione intermedia
                cv::Mat tmp = image.clone();
                for (auto& p : points)
                {
                    double d = Distance2Line(best_A, best_B, p);
                    cv::Scalar col = (d < T ? cv::Scalar(0,255,0) : cv::Scalar(0,0,255));
                    cv::circle(tmp, p, 2, col, -1);
                }
                cv::line(tmp, best_A, best_B, cv::Scalar(0,255,255), 2);

                std::string saveName = "../out/bestmodel_" + std::to_string(it) + ".png";
                cv::imwrite(saveName, tmp);

                std::cout << "[UPDATE] Iterazione " << it
                          << "  Inlier: " << best_inliers
                          << " → Salvato: " << saveName << std::endl;
            }
        }

        std::cout << "Best model: " << best_inliers << " inlier" << std::endl;

        // -------------------------------------------------------------
        // DISEGNO FINALE
        // -------------------------------------------------------------
        for (auto& p : points) {
            cv::Scalar col = (Distance2Line(best_A, best_B, p) < T ?
                              cv::Scalar(0,255,0) : cv::Scalar(0,0,255));
            cv::circle(out, p, 2, col, -1);
        }
        cv::line(out, best_A, best_B, cv::Scalar(0,255,255), 2);

        cv::imshow("output image", out);

        // Salvataggio finale
        std::string finalName = "../out/result_" + std::to_string(frame_number) + ".png";
        cv::imwrite(finalName, out);
        std::cout << "Salvata immagine finale: " << finalName << std::endl;

        // gestione tastiera
        unsigned char key = cv::waitKey(args.wait_t);
        if (key == 'q') exit_loop = true;

        frame_number++;
    }

    return 0;
}


// ================================================================
// PARSE INPUTS
// ================================================================
#include <unistd.h>
bool ParseInputs(ArgumentList& args, int argc, char **argv) {
    int c;
    args.dist_t = 30;
    args.wait_t = 0;

    while ((c = getopt (argc, argv, "hi:t:d:p:")) != -1)
        switch (c)
        {
            case 'd': args.dist_t = atoi(optarg); break;
            case 't': args.wait_t = atoi(optarg); break;
            case 'p': args.points_name = optarg; break;
            case 'i': args.image_name = optarg; break;
            case 'h':
            default:
                std::cout <<
                "Options:\n"
                " -i <img>    immagine input\n"
                " -p <pts>    file dei punti\n"
                " -d <dist>   soglia RANSAC (default 30)\n"
                " -t <ms>     waitKey ms (default 0)\n";
                return false;
        }
    return true;
}

