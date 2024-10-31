#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <ctime>

long long int power(long long int a, long long int b, long long int c,
                    long long int P)
{
    if (c == 1) return b;
    if (c == 0) return 1;

    else
        return power(a, (a * b) % P, c - 1, P);
}

int manualDHExchange() {
    unsigned int prime = 23;
    unsigned int gen = 2;
    std::srand(std::time(0)); 

    std::cout << "(g, p) = (" << std::to_string(gen) << ", " << std::to_string(prime) << ")" << std::endl;
    int our_private_key = std::rand() % prime;
    std::cout << "Our private: " << std::to_string(our_private_key) << std::endl;
    unsigned int our_public = power(gen, gen, our_private_key, prime);
    std::cout << "Here's our public key, have fun: " << std::to_string(our_public) << std::endl;

    unsigned int alice_exchange;
    std::cin >> alice_exchange;

    if (alice_exchange >= prime) {
        std::cout << "Silly! There's no way you could've gotten that";
    }

    unsigned int shared_secret = power(alice_exchange, alice_exchange, our_private_key, prime);
    std::cout << "Shared secret: " << std::to_string(shared_secret) << std::endl;

    return 0;
}
