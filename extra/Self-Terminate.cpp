#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ptrace.h>
#endif

// Function to create a deletion script that runs independently
void selfDelete() {
    std::ofstream script("delete_self.sh");
    script << "#!/bin/bash\n";
    script << "sleep 2\n";  // Delay to ensure the program has fully terminated
    script << "rm -f Self-Terminate.cpp\n";  // Delete the source file
    script << "rm -f ./a.out\n";  // Delete the executable (or whatever it is called)
    script << "rm -- \"$0\"\n";  // Delete this script itself
    script.close();

    // Make the script executable
    chmod("delete_self.sh", S_IRWXU);

    // Fork a new process to run the deletion script in the background
    if (fork() == 0) {
        // Run the deletion script and wait until the main process exits
        execl("/bin/sh", "sh", "-c", "./delete_self.sh", (char *) NULL);
        exit(0);
    }
}

// Cross-platform debugger detection
bool isDebuggerActive() {
#ifdef _WIN32
    return IsDebuggerPresent();  // Windows API
#elif __linux__
    std::ifstream statusFile("/proc/self/status");
    std::string line;
    while (std::getline(statusFile, line)) {
        if (line.find("TracerPid") != std::string::npos) {
            int tracerPid = std::stoi(line.substr(line.find_last_of("\t ") + 1));
            if (tracerPid != 0) {
                return true;  // A debugger is attached
            }
        }
    }
    return false;
#elif __APPLE__
    if (ptrace(PT_DENY_ATTACH, 0, 0, 0) == -1) {
        return true;  // Deny ptrace, means a debugger is likely attached
    }
    return false;
#else
    return false;  // Platform not supported
#endif
}

// Cross-platform virtual machine detection using CPUID
bool isVirtualMachine() {
#ifdef _WIN32
    int cpuInfo[4];
    __cpuid(cpuInfo, 1);
    bool vmDetected = (cpuInfo[2] >> 31) & 1;  // Check hypervisor flag in ECX
    return vmDetected;
#elif defined(__x86_64__) || defined(__i386__)
    unsigned int eax, ebx, ecx, edx;
    eax = 1;
    __asm__ volatile ("cpuid"
                      : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                      : "a"(eax));
    bool vmDetected = (ecx >> 31) & 1;  // Check hypervisor flag in ECX
    return vmDetected;
#else
    return false;  // Unsupported CPU architecture
#endif
}

int main() {
    // Check if a debugger is present
    if (isDebuggerActive()) {
        selfDelete();
        exit(1);
    }

    // Check if running inside a virtual machine
    if (isVirtualMachine()) {
        selfDelete();
        exit(1);
    }

    // Simulate some work
    // Your actual program logic can be placed here

    // After the program finishes, self-delete and delete the source file
    selfDelete();
    return 0;
}
