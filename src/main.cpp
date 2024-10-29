#include <iostream>
#include "detection.h"

int main (int argc, char *argv[]) {
    if (detectVM()) {
        std::cout << "You are in a VM" << std::endl;
    } else {
        std::cout << "You are NOT in a VM" << std::endl;
    }
    
    return 0;
}
