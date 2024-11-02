#pragma once

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <fstream>
#include <vector>


#include <string>
#include <openssl/sha.h>

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}


// Encrypted with  openssl enc -aes-256-cbc -p -in howdy.txt -out howdy.enc -k PASSWORD
std::vector<uint8_t> aes_decrypt(const uint8_t* message, size_t message_len, const std::string& password)
{
    const uint8_t* salt = message + 8;
    const uint8_t* ciphertext = salt + 8;
    const size_t cipher_len = message_len - 16;

    // Key and IV generation using EVP_BytesToKey
    uint8_t key[32], iv[16];
    int err = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), salt, (const uint8_t*)password.c_str(), password.size(), 1, key, iv);
    if (!err) handleErrors();

    // Buffer for the decrypted text
    std::vector<uint8_t> plaintext(cipher_len);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) handleErrors();

    int len;
    int plaintext_len;

    // Decrypt the ciphertext
    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext, cipher_len)) handleErrors();
    plaintext_len = len;

    // Finalize the decryption
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) handleErrors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    
    return plaintext;
}

// std::vector<uint8_t> sha256(const std::string &str)
// {
//     std::vector<uint8_t> hash(SHA256_DIGEST_LENGTH);
//     SHA256_CTX sha256;
//     SHA256_Init(&sha256);
//     SHA256_Update(&sha256, str.c_str(), str.size());
//     SHA256_Final(hash.data(), &sha256);
//     return hash;
// }
