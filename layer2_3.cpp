#include <csignal>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include "dhexchange.h"
#include "crypto.h"
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/aes.h>
#include <openssl/err.h>

void handler(int _signum) {
   exit(1);
}

int main (int argc, char *argv[]) {
   signal(SIGTSTP, handler);
   signal(SIGKILL, handler);
   signal(SIGTERM, handler);

   unsigned int shared_secret = -1; 

   shared_secret = manualDHExchange();
   std::vector<uint8_t> shared_secret_as_key = sha256(std::to_string(shared_secret));

   // Get decryption key from user, where the key is expected to be encrypted by the user using the shared secret
   std::cout << "Welcome to layer 3. You're almost there!" << std::endl;
   std::cout << "All I need is the key to decrypt the payload. But I need to make sure I can trust the sender. Please send me the key safely." << std::endl;   
   std::cout << "Enter the name of the file I can read your encrypted data from: ";

   std::string filename;
   std::cin >> filename;

   std::ifstream encryptedKeyFile(filename, std::ios::binary);
   if (!encryptedKeyFile) {
      std::cout << "Could not read your file. Exiting now..." << std::endl;
      exit(1);
   }

       // Read the ciphertext
   std::vector<unsigned char> ciphertext((std::istreambuf_iterator<char>(encryptedKeyFile)), std::istreambuf_iterator<char>());
   encryptedKeyFile.close();

   std::vector<uint8_t> plaintext(ciphertext.size());

   unsigned char iv[16] = {0};

   EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
   if (!ctx) handleErrors();

   if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, shared_secret_as_key.data(), iv)) handleErrors();

   int len;
   int plaintext_len;

   // Decrypt the ciphertext
   if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size())) handleErrors();
   plaintext_len = len;

   // Finalize the decryption
   if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) handleErrors();
   plaintext_len += len;

   EVP_CIPHER_CTX_free(ctx);

   // Use key to decrypt the payload
   //

   return 0;
}
