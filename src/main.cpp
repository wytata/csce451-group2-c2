#include <iostream>

#include "detection.h"
#include "crypto.h"
#include "embed.h"
#include "MemoryFile.h"


int main (int argc, char *argv[]) {
    std::cout << "What's the password: ";
    std::string password;
    std::cin >> password;

    auto dec_elf = aes_decrypt(embed_enc, embed_enc_len, password);

    return 
        MemFile("layers2-3")
            .write(dec_elf)
            .execute();
}
