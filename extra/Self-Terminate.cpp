#include <windows.h>
#include <iostream>
#include <fstream>
#include <intrin.h>

// Check if a debugger is attached using IsDebuggerPresent
bool isDebuggerActive() {
    return IsDebuggerPresent();
}

// Check for the presence of a virtual machine using CPUID instruction
bool isVirtualMachine() {
    int cpuInfo[4] = { -1 };
    __cpuid(cpuInfo, 1);  // CPU info function 1

    bool vmDetected = (cpuInfo[2] >> 31) & 1;  // Check the 31st bit in ECX for the hypervisor flag
    return vmDetected;
}

// Self-terminate by launching a batch file to delete the executable
void selfDelete() {
    char tempPath[MAX_PATH];
    GetTempPath(MAX_PATH, tempPath);
    char batchFile[MAX_PATH];
    GetTempFileName(tempPath, "del", 0, batchFile);

    std::ofstream batch(batchFile);
    batch << ":Repeat\n";
    batch << "del \"" << __argv[0] << "\"\n";  // Try to delete the executable
    batch << "if exist \"" << __argv[0] << "\" goto Repeat\n";
    batch << "del \"%0\"\n";  // Delete the batch file itself
    batch.close();

    // Execute the batch file in hidden mode
    ShellExecute(NULL, "open", batchFile, NULL, NULL, SW_HIDE);
}

int main() {
    std::cout << "Starting program..." << std::endl;

    // Check if a debugger is present
    if (isDebuggerActive()) {
        std::cout << "Debugger detected! Terminating program..." << std::endl;
        selfDelete();
        exit(1);
    }

    // Check if running inside a virtual machine
    if (isVirtualMachine()) {
        std::cout << "Virtual machine detected! Terminating program..." << std::endl;
        selfDelete();
        exit(1);
    }

    std::cout << "No debugger or virtual machine detected. Program continues..." << std::endl;
    
    // Rest of your code here...
    
    return 0;
}
