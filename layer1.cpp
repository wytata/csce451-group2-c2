#include <csignal>
#include <iostream>
#include "detection.h"
#include "crypto.h"
#include <unistd.h>
#include <sys/wait.h>

void handler(int _signum) {
    std::remove("file");
    exit(1);
}

int main (int argc, char *argv[]) {
    signal(SIGTSTP, handler);
    signal(SIGKILL, handler);
    signal(SIGTERM, handler);

    std::cout << "What's the password: ";
    std::string password;
    std::cin >> password;

    std::ifstream encryptedFile("layer2_3.enc", std::ios::binary);
    if (!encryptedFile) {
        std::cerr << "Could not open encrypted file." << std::endl;
        return 1;
    }

    // Read the ciphertext
    std::vector<unsigned char> ciphertext((std::istreambuf_iterator<char>(encryptedFile)), std::istreambuf_iterator<char>());
    encryptedFile.close();
        

    std::vector<uint8_t> dec = aes_decrypt(ciphertext.data(), ciphertext.size(), password);
    std::ofstream decryptedFile("layer2_3.dec", std::ios::binary);
    decryptedFile.write(reinterpret_cast<char*>(dec.data()), dec.size());
    decryptedFile.close();

    pid_t pid = fork();

    if (pid == -1) {
        std::cout << "fork() failed" << std::endl;
        exit(1);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        execv("./layer2_3.dec", NULL);
    }

    return 0;
}
