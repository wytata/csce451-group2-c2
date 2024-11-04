#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <ctime>

static long long int power(long long int a, long long int b, long long int c,
                    long long int P)
{
    if (c == 1) return b;
    if (c == 0) return 1;

    else
        return power(a, (a * b) % P, c - 1, P);
}

unsigned int manualDHExchange() {
    unsigned int prime = 1987;
    unsigned int gen = 2;
    std::srand(std::time(0)); 

    std::cout << "(g, p) = (" << gen << ", " << prime << ")" << std::endl;

    unsigned int our_private_key = std::rand() % prime;
    std::cout << "Our private key: " << our_private_key << std::endl;
    unsigned int our_public = power(gen, gen, our_private_key, prime);
    std::cout << "Here's our public key, have fun: " << our_public << std::endl;
    std::cout << "Your turn! Give me your public key: ";

    unsigned int alice_exchange;
    std::cin >> alice_exchange;

    if (alice_exchange >= prime) {
        std::cout << "Silly! There's no way you could've gotten that";
    }

    unsigned int shared_secret = power(alice_exchange, alice_exchange, our_private_key, prime);
    std::cout << "shared secret: " << shared_secret << std::endl;

    return shared_secret;
}
