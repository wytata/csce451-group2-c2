#include <csignal>
#include <iostream>
#include "detection.h"
#include "dhexchange.h"

void handler(int signum) {
    std::remove("file");
    exit(1);
}

int main (int argc, char *argv[]) {
    signal(SIGTSTP, handler);
    signal(SIGKILL, handler);
    signal(SIGTERM, handler);

    for(;;);
    if (detectVM()) {
        std::cout << "You are in a VM" << std::endl;
    } else {
        std::cout << "You are NOT in a VM" << std::endl;
    }

    return 0;
}
