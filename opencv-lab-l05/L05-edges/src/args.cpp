
#include "args.hpp"
#include <iostream>
#include <unistd.h>

bool ParseInputs(ArgumentList& args, int argc, char** argv){
    int c;
    // defaults already set in struct
    while((c = getopt(argc, argv, "hi:t:s:k:L:H:")) != -1){
        switch(c){
            case 'i': args.image_name = optarg; break;
            case 't': args.wait_t = std::atoi(optarg); break;
            case 's': args.sigma = std::atof(optarg); break;
            case 'k': args.kernel_size = std::atoi(optarg); break;
            case 'L': args.low_th = std::atof(optarg); break;
            case 'H': args.high_th = std::atof(optarg); break;
            case 'h':
            default:
                std::cout << "Uso: " << argv[0] << " -i <immagine> [-s sigma] [-k kernel] [-L low] [-H high] [-t wait]\n";
                std::cout << "Note: kernel deve essere dispari. L'immagine viene letta in scala di grigi.\n";
                return false;
        }
    }
    if(args.image_name.empty()){
        std::cerr << "Specificare -i <immagine>.\n";
        return false;
    }
    if(args.kernel_size % 2 == 0){
        std::cerr << "kernel_size deve essere dispari. Uso kernel_size+1.\n";
        args.kernel_size += 1;
    }
    return true;
}
