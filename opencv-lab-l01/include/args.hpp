
#pragma once
#include <string>
#include <vector>
#include <optional>
#include <unordered_set>

struct CropSpec {
    int u = 100; // column (x)
    int v = 100; // row (y)
    int w = 100; // width
    int h = 100; // height
};

struct Settings {
    std::string imagePath;
    int waitMs = 0;
    int padding = 5;
    bool randomCrop = false;
    CropSpec crop;
    std::unordered_set<std::string> exercises; // names
    bool runAll = false;
};

// Parse a simple CLI with flags like: --image path --wait 30 --pad 8 --crop 100,150,120,80 --random-crop --exercises down2x,flipH
// Returns std::nullopt on error (and prints usage)
std::optional<Settings> parseArgs(int argc, char** argv);

void printUsage(const char* prog);
