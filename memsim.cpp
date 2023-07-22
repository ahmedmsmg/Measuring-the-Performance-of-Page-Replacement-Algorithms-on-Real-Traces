#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <cstring>
#include "policies.h"
#include <cstddef> // for nullptr


int main(int argc, char *argv[])
{
    if(argc < 4) {
        std::cerr << "Insufficient arguments provided. Expected at least 4, but received " << argc-1 << ".\n";
        return -1;
    }

    std::string algorithm = argv[3];
    std::string tracefile = argv[1];
    int nframes = std::atoi(argv[2]);

    try {
        if (algorithm == "lru") {
            lru(tracefile.c_str(), nframes, argv[4]);
        } 
        else if (algorithm == "fifo") {
            fifo(tracefile.c_str(), nframes, argv[4]);
        } 
        else if (algorithm == "vms" && argc == 6) {
            int percent = std::atoi(argv[4]);
            segmented_fifo(tracefile.c_str(), nframes, percent, argv[5]);
        } 
        else {
            throw std::invalid_argument("Algorithm '" + algorithm + "' is not supported. Please select 'lru', 'fifo' or 'vms'.");
        }
    }
    catch(const std::invalid_argument& e) {
        std::cerr << e.what() << "\n";
        return -1;
    }
}
