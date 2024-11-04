#include <csignal>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include "dhexchange.h"
#include "secret.h"
#include "crypto.h"

#include <cstring>

void handler(int _signum) {
   exit(1);
}

int main (int argc, char *argv[]) {
   signal(SIGTSTP, handler);
   signal(SIGKILL, handler);
   signal(SIGTERM, handler);

   unsigned int shared_secret = -1; 

   shared_secret = manualDHExchange();

   // Get decryption key from user, where the key is expected to be encrypted by the user using the shared secret
   std::cout << "Welcome to layer 3. You're almost there!" << std::endl;
   std::cout << "All I need is the password to decrypt the payload. But I need to make sure I can trust the sender. Please send me the password safely. (it can have spaces)" << std::endl;   
   std::cout << "Enter the name of the file I can read your encrypted data from: ";

   std::string filename;
   std::cin >> filename;

   std::ifstream encryptedKeyFile(filename, std::ios::binary);
   if (!encryptedKeyFile) {
      std::cout << "Could not read your file. Exiting now..." << std::endl;
      exit(1);
   }

   // Read the ciphertext
   std::vector<uint8_t> ciphertext((std::istreambuf_iterator<char>(encryptedKeyFile)), std::istreambuf_iterator<char>());
   encryptedKeyFile.close();

   auto passwd = aes_decrypt(ciphertext.data(), ciphertext.size(), std::to_string(shared_secret));   
   auto secret = aes_decrypt(secret_enc, secret_enc_len, passwd);

   printf("%s\n", secret.data());

   return 0;
}
