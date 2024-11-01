#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <unistd.h>
#include <sys/wait.h>
#include "crypto.h"

int main() {
    // The key and IV from OpenSSL generation
    uint8_t key[32] = { 0x9F, 0xEA, 0x8D, 0x7D, 0x44, 0xF2, 0xE8, 0xB2, 0xF6, 0x0A, 0x8E, 0x8F, 0x57, 0xAE, 0x85, 0xB1,
                              0xB1, 0xE9, 0x7E, 0x8D, 0x64, 0xCC, 0x91, 0x88, 0x5A, 0x99, 0xC6, 0xC2, 0x38, 0xFB, 0x11, 0x22 };

    // Open the encrypted file
    std::ifstream encryptedFile("inner.encrypted", std::ios::binary);
    std::vector<uint8_t> ciphertext((std::istreambuf_iterator<char>(encryptedFile)), std::istreambuf_iterator<char>());
    encryptedFile.close();

    std::vector<uint8_t> plaintext = decrypt(ciphertext.data(), ciphertext.size(), key, iv);
    int plaintext_len = plaintext.size();

    // Write the decrypted text to a file
    std::ofstream decryptedFile("inner.decrypted", std::ios::binary);
    decryptedFile.write(reinterpret_cast<char*>(plaintext.data()), plaintext_len);
    std::filesystem::permissions("inner.decrypted", std::filesystem::perms::all);
    decryptedFile.close();

    std::cout << "Decryption complete. Decrypted binary saved to inner.decrypted" << std::endl;

    pid_t pid = fork();
    if (pid == -1) {
        std::cout << "Fork failed" << std::endl;
        exit(1);
    } else if (pid == 0) {
        execv("./inner.decrypted", NULL);
    } else {
        wait(NULL);
        //waitpid(layer2_pid, NULL, 0);
    }

    return 0;
}

