
#include "args.hpp"
#include "io.hpp"
#include "background.hpp"

#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
#include <cstdio>

using namespace std::string_literals;

static bool hasPattern(const std::string& s){ return s.find('%') != std::string::npos; }

void printHelp(const char* prog){
    std::cout << "Usage:\n"
              << "  " << prog << " --image <seq_pattern> --mode <prev|mean|ema> [options]\n\n"
              << "Options:\n"
              << "  --image <pattern>   Sequence path (printf-style, e.g. ../data/seq_%03d.png)\n"
              << "  --mode <name>       prev | mean | ema (default prev)\n"
              << "  --threshold <k>     Threshold for foreground mask (default 50)\n"
              << "  --window <N>        Window size for running mean (default 10)\n"
              << "  --alpha <a>         EMA factor in [0,1] (default 0.5)\n"
              << "  --wait <ms>         Wait between frames (default 0)\n"
              << "  --no-save           Do not write outputs to disk (default: save)\n"
              << "  -h, --help          Show this help and exit\n";
}

std::optional<Cli> parseCli(int argc, char** argv){
    Cli c;
    for(int i=1;i<argc;++i){
        std::string a = argv[i];
        auto need=[&](int n){ return (i+n) < argc; };
        if(a=="-h"||a=="--help"){ printHelp(argv[0]); return std::nullopt; }
        else if(a=="--image" && need(1)) c.imagePattern = argv[++i];
        else if(a=="--mode" && need(1))  c.mode = argv[++i];
        else if(a=="--threshold" && need(1)) c.threshold = std::stoi(argv[++i]);
        else if(a=="--window" && need(1))    c.window = std::stoi(argv[++i]);
        else if(a=="--alpha" && need(1))     c.alpha = std::stod(argv[++i]);
        else if(a=="--wait" && need(1))      c.waitMs = std::stoi(argv[++i]);
        else if(a=="--no-save")              c.save = false;
        else { std::cerr<<"Unknown arg: "<<a<<"\n"; printHelp(argv[0]); return std::nullopt; }
    }
    if(c.imagePattern.empty()){ std::cerr<<"--image is required\n"; return std::nullopt; }
    if(!hasPattern(c.imagePattern)){
        std::cerr<<"Error: --image must be a sequence pattern like seq_%03d.png\n";
        return std::nullopt;
    }
    if(c.mode!="prev" && c.mode!="mean" && c.mode!="ema"){
        std::cerr<<"Error: invalid --mode value\n"; return std::nullopt;
    }
    if(c.alpha < 0.0 || c.alpha > 1.0){
        std::cerr<<"Error: --alpha must be in [0,1]\n"; return std::nullopt;
    }
    return c;
}

int main(int argc, char** argv){
    auto opt = parseCli(argc, argv);
    if(!opt) return 1;
    auto cfg = *opt;

    std::cout << "L03 Background Subtraction - mode=" << cfg.mode << "\n";

    int idx = 0;
    cv::Mat bg, frame, fg, meanBg;
    std::vector<cv::Mat> history;

    // Ensure output directories exist
    ensureDir("prev");
    ensureDir("mean");
    ensureDir("ema");
    ensureDir("out");

    while(true){
        std::string path = formatPattern(cfg.imagePattern, idx);
        frame = loadGray(path);
        if(frame.empty()){
            if(idx==0){
                std::cerr << "Cannot open first frame: " << path << "\n";
                return 1;
            }
            std::cout << "End of sequence at index " << idx << "\n";
            break;
        }

        if(cfg.mode=="prev"){
            fg = bgPrevFrame(frame, bg, cfg.threshold);
            show("PrevFrame - BG", bg, 1);
            show("PrevFrame - FG", fg, cfg.waitMs);
            if(cfg.save){
                char bgbuf[64], fgbuf[64];
                std::snprintf(bgbuf, sizeof(bgbuf), "prev/bg_%06d.png", idx);
                std::snprintf(fgbuf, sizeof(fgbuf), "prev/fg_%06d.png", idx);
                imwriteSafe(bgbuf, bg);
                imwriteSafe(fgbuf, fg);
            }
        }
        else if(cfg.mode=="mean"){
            fg = bgRunningMean(frame, history, cfg.window, cfg.threshold, meanBg);
            show("Running Mean - BG", meanBg, 1);
            show("Running Mean - FG", fg, cfg.waitMs);
            if(cfg.save){
                char bgbuf[64], fgbuf[64];
                std::snprintf(bgbuf, sizeof(bgbuf), "mean/bg_%06d.png", idx);
                std::snprintf(fgbuf, sizeof(fgbuf), "mean/fg_%06d.png", idx);
                imwriteSafe(bgbuf, meanBg);
                imwriteSafe(fgbuf, fg);
            }
        }
        else if(cfg.mode=="ema"){
            fg = bgEMA(frame, bg, cfg.alpha, cfg.threshold);
            show("EMA - BG", bg, 1);
            show("EMA - FG", fg, cfg.waitMs);
            if(cfg.save){
                char bgbuf[64], fgbuf[64];
                std::snprintf(bgbuf, sizeof(bgbuf), "ema/bg_%06d.png", idx);
                std::snprintf(fgbuf, sizeof(fgbuf), "ema/fg_%06d.png", idx);
                imwriteSafe(bgbuf, bg);
                imwriteSafe(fgbuf, fg);
            }
        }

        int key = cv::waitKey(cfg.waitMs);
        if(key=='q' || key=='Q') break;
        ++idx;
    }

    return 0;
}
