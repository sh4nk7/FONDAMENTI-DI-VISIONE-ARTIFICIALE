
#include "args.hpp"
#include "io.hpp"
#include "ops.hpp"

#include <opencv2/highgui.hpp>
#include <iostream>
#include <sstream>
#include <random>

static std::vector<std::string> split(const std::string& s, char delim){
    std::vector<std::string> out; std::stringstream ss(s); std::string item;
    while(std::getline(ss,item,delim)) if(!item.empty()) out.push_back(item);
    return out;
}

void printUsage(const char* prog){
    std::cout << "Usage:\n"
              << "  " << prog << " --image <path> [options]\n\n"
              << "Options:\n"
              << "  --image <path>          Input image (supports %0xd frame pattern)\n"
              << "  --wait <ms>             Wait time between frames (default 0)\n"
              << "  --pad <n>               Zero padding size (default 5)\n"
              << "  --crop u,v,w,h          Crop rectangle (default 100,100,100,100)\n"
              << "  --random-crop           Enable random crop each frame\n"
              << "  --exercises list        Comma-separated list of exercises to run\n"
              << "                          Available: down2x, downRows, downCols, flipH, flipV,\n"
              << "                                     crop, pad, shuffleBlocks, shuffleChannels,\n"
              << "                                     debayerDownG, debayerLum, debayerSimple, debayerSimpleDown\n"
              << "  --all                   Run all exercises\n"
              << "  -h, --help              Show this help\n";
}

std::optional<Settings> parseArgs(int argc, char** argv){
    if(argc<2){ printUsage(argv[0]); return std::nullopt; }
    Settings s;
    for(int i=1;i<argc;++i){
        std::string a = argv[i];
        auto need = [&](int n){ return (i+n) < argc; };
        if(a=="-h"||a=="--help"){ printUsage(argv[0]); return std::nullopt; }
        else if(a=="--image" && need(1)){ s.imagePath = argv[++i]; }
        else if(a=="--wait" && need(1)){ s.waitMs = std::stoi(argv[++i]); }
        else if(a=="--pad" && need(1)){ s.padding = std::stoi(argv[++i]); }
        else if(a=="--crop" && need(1)){
            auto toks = split(argv[++i], ',');
            if(toks.size()==4){ s.crop.u=std::stoi(toks[0]); s.crop.v=std::stoi(toks[1]); s.crop.w=std::stoi(toks[2]); s.crop.h=std::stoi(toks[3]); }
            else { std::cerr<<"Invalid --crop format\n"; return std::nullopt; }
        }
        else if(a=="--random-crop"){ s.randomCrop=true; }
        else if(a=="--exercises" && need(1)){
            auto toks = split(argv[++i], ',');
            for(auto& t: toks) s.exercises.insert(t);
        }
        else if(a=="--all"){ s.runAll=true; }
        else {
            std::cerr<<"Unknown arg: "<<a<<"\n";
            printUsage(argv[0]);
            return std::nullopt;
        }
    }
    if(s.imagePath.empty()){ std::cerr<<"--image is required\n"; return std::nullopt; }
    return s;
}

int main(int argc, char** argv){
    auto opt = parseArgs(argc, argv);
    if(!opt) return 1;
    auto cfg = *opt;

    std::cout << "L01 rewrite - OpenCV basic ops\n";

    int frameIdx = 0;
    bool multi = cfg.imagePath.find('%') != std::string::npos;
    std::string path = cfg.imagePath;
    std::mt19937 rng{std::random_device{}()};

    while(true){
        char buf[512];
        if(multi) std::snprintf(buf, sizeof(buf), cfg.imagePath.c_str(), frameIdx);
        else std::snprintf(buf, sizeof(buf), "%s", cfg.imagePath.c_str());

        std::string fname(buf);
        std::cout << "Opening " << fname << "\n";
        cv::Mat img = loadImageSmart(fname);
        if(img.empty()){
            std::cerr << "Cannot open " << fname << "\n";
            return 1;
        }

        showAndWait("Original", img, 1, false);

        auto doRun = [&](const std::string& name){
            return cfg.runAll || cfg.exercises.count(name)>0;
        };

        // --- DOWN 2x
        if(doRun("down2x")){
            auto out = downsample2x(img);
            showAndWait("Downsample 2x", out, cfg.waitMs, true);
        }

        // --- DOWN rows
        if(doRun("downRows")){
            auto out = downsampleRows2x(img);
            showAndWait("Downsample rows 2x", out, cfg.waitMs, true);
        }

        // --- DOWN cols
        if(doRun("downCols")){
            auto out = downsampleCols2x(img);
            showAndWait("Downsample cols 2x", out, cfg.waitMs, true);
        }

        // --- FLIP H
        if(doRun("flipH")){
            auto out = flipHorizontal(img);
            showAndWait("Flip horizontal", out, cfg.waitMs, true);
        }

        // --- FLIP V
        if(doRun("flipV")){
            auto out = flipVertical(img);
            showAndWait("Flip vertical", out, cfg.waitMs, true);
        }

        // --- CROP
        if(doRun("crop")){
            int u=cfg.crop.u, v=cfg.crop.v, w=cfg.crop.w, h=cfg.crop.h;
            if(cfg.randomCrop){
                std::uniform_int_distribution<int> U(0, std::max(0, img.cols-1));
                std::uniform_int_distribution<int> V(0, std::max(0, img.rows-1));
                u = U(rng); v = V(rng);
                std::uniform_int_distribution<int> W(1, std::max(1, img.cols-u));
                std::uniform_int_distribution<int> H(1, std::max(1, img.rows-v));
                w = W(rng); h = H(rng);
            }
            auto out = cropRegion(img, u, v, w, h);
            showAndWait("Crop", out, cfg.waitMs, true);
        }

        // --- PADDING
        if(doRun("pad")){
            auto out = addZeroPadding(img, cfg.padding);
            showAndWait("Zero padding", out, cfg.waitMs, true);
        }

        // --- SHUFFLE BLOCKS
        if(doRun("shuffleBlocks")){
            auto out = shuffleBlocks4(img);
            showAndWait("Shuffle 2x2 blocks", out, cfg.waitMs, true);
        }

        // --- SHUFFLE CHANNELS
        if(doRun("shuffleChannels") && img.type()==CV_8UC3){
            auto out = shuffleChannels(img);
            showAndWait("Shuffle channels", out, cfg.waitMs, true);
        }

        // --- DEBAYER
        auto pat = detectBayer(fname);
        if(pat != Bayer::NONE){
            if(doRun("debayerDownG")){
                auto out = debayerDownsampleG(img, pat);
                showAndWait("Debayer Down G", out, cfg.waitMs, true);
            }
            if(doRun("debayerLum")){
                auto out = debayerLuminance(img, pat);
                showAndWait("Debayer Luminance", out, cfg.waitMs, true);
            }
            if(doRun("debayerSimple")){
                auto out = debayerSimple(img, pat);
                showAndWait("Debayer Simple", out, cfg.waitMs, true);
            }
            if(doRun("debayerSimpleDown")){
                auto out = debayerSimpleDown(img, pat);
                showAndWait("Debayer Simple Down", out, cfg.waitMs, true);
            }
        }

        int key = cv::waitKey(cfg.waitMs);
        std::cout << "key: " << key << std::endl;
        if(key=='q' || key=='Q') break;

        ++frameIdx;
        if(!multi) break; // single image: exit after one iteration
    }

    return 0;
}
