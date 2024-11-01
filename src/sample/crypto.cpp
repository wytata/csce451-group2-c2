#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <fstream>
#include <vector>

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

std::vector<unsigned char> decrypt(const unsigned char* message, int messageLength, const unsigned char* key, const unsigned char* iv) {
    // Buffer for the decrypted text
    std::vector<unsigned char> plaintext(messageLength);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) handleErrors();

    int len;
    int plaintext_len;

    // Decrypt the ciphertext
    //if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size())) handleErrors();
    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, message, messageLength)) handleErrors();
    plaintext_len = len;

    // Finalize the decryption
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) handleErrors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    
    return plaintext;
}
