#include "BlobSimulation.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    unsigned int width = 1280;
    unsigned int height = 720;
    
    if (argc >= 3) {
        width = std::atoi(argv[1]);
        height = std::atoi(argv[2]);
    }
    
    try {
        BlobSimulation simulation(width, height);
        simulation.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}