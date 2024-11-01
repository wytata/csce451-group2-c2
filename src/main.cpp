#include <csignal>
#include <iostream>
#include "detection.h"
#include "dhexchange.h"

#include "crypto.h"
#include "binary.h"

// extern "C" const char howdy_enc[];
// extern "C" const size_t howdy_enc_len;

void handler(int _signum) {
    std::remove("file");
    exit(1);
}

int main (int argc, char *argv[]) {
    signal(SIGTSTP, handler);
    signal(SIGKILL, handler);
    signal(SIGTERM, handler);


    // printf("Password: %X\n", .data());
    // for (uint8_t byte : sha256("PASSWORD")) printf("%X", byte);
    // printf("\n");

    // exit(0);

    const unsigned char PW[] = "PASSWORD";
    

    std::vector<uint8_t> dec = aes_decrypt(howdy_enc, howdy_enc_len, PW);


    printf("Output: %s\n", dec.data());

    for(;;);
    if (detectVM()) {
        std::cout << "You are in a VM" << std::endl;
    } else {
        std::cout << "You are NOT in a VM" << std::endl;
    }

    return 0;
}
