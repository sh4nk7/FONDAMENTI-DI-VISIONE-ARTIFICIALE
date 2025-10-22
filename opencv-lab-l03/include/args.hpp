
#pragma once
#include <string>
#include <optional>

struct Cli {
    std::string imagePattern; // sequence path, supports %0xd
    std::string mode = "prev"; // prev | mean | ema
    int waitMs = 0;
    int threshold = 50;
    int window = 10;      // for mean
    double alpha = 0.5;   // for ema
    bool save = true;     // save outputs
};

std::optional<Cli> parseCli(int argc, char** argv);
void printHelp(const char* prog);
