#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/err.h>

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

int main() {
    // The key and IV from OpenSSL generation
    unsigned char key[32] = { 0x9F, 0xEA, 0x8D, 0x7D, 0x44, 0xF2, 0xE8, 0xB2, 0xF6, 0x0A, 0x8E, 0x8F, 0x57, 0xAE, 0x85, 0xB1,
                              0xB1, 0xE9, 0x7E, 0x8D, 0x64, 0xCC, 0x91, 0x88, 0x5A, 0x99, 0xC6, 0xC2, 0x38, 0xFB, 0x11, 0x22 };
    unsigned char iv[16] = { 0x8C, 0xF1, 0xF3, 0xE3, 0x77, 0xA4, 0xC3, 0xEB, 0xE4, 0xD3, 0xC6, 0xF3, 0x83, 0x45, 0x99, 0xB2 };

    // Open the encrypted file
    std::ifstream encryptedFile("inner.encrypted", std::ios::binary);
    std::vector<unsigned char> ciphertext((std::istreambuf_iterator<char>(encryptedFile)), std::istreambuf_iterator<char>());
    encryptedFile.close();

    // Buffer for the decrypted text
    std::vector<unsigned char> plaintext(ciphertext.size());

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) handleErrors();

    int len;
    int plaintext_len;

    // Decrypt the ciphertext
    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size())) handleErrors();
    plaintext_len = len;

    // Finalize the decryption
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) handleErrors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    // Write the decrypted text to a file
    std::ofstream decryptedFile("inner.decrypted", std::ios::binary);
    decryptedFile.write(reinterpret_cast<char*>(plaintext.data()), plaintext_len);
    std::filesystem::permissions("inner.decrypted", std::filesystem::perms::all);
    decryptedFile.close();

    std::cout << "Decryption complete. Decrypted binary saved to inner.decrypted" << std::endl;

    return 0;
}

