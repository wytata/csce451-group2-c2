#include <algorithm>
#include <iostream>
#include "detection.h"
#include <fstream>

bool detectVM() {
    std::ifstream cpuinfo("/proc/cpuinfo", std::ifstream::in);
    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.find("hypervisor") != std::string::npos) {
            return true;
        }
    }
    cpuinfo.close();
    return false;
}
