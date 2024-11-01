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

std::vector<uint8_t> aes_decrypt(const uint8_t* const message, size_t messageLength, const uint8_t* const key)
{
    // Buffer for the decrypted text
    std::vector<uint8_t> plaintext(messageLength);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL)) handleErrors();

    int len;
    int plaintext_len;

    // Decrypt the ciphertext
    //if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size())) handleErrors();
    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, message, messageLength)) handleErrors();
    plaintext_len = len;

    // Finalize the decryption
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) handleErrors();
    plaintext_len += len;
    plaintext.resize(plaintext_len);

    EVP_CIPHER_CTX_free(ctx);

    
    return plaintext;
}

std::vector<uint8_t> sha256(const std::string &str)
{
    std::vector<uint8_t> hash(SHA256_DIGEST_LENGTH);
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash.data(), &sha256);
    return hash;
}
