#include <iostream>
#include <cstdlib>
#include <ctime>

int64_t keygen() {
    // Seed the random number generator with the current time
    srand(time(0)); 

    // Generate a random number between 1 and 100
    return rand() % 10000 + 1; 
}


int main() {
    int64_t key = keygen();
    printf("%ld\n", key);

    return 0;
}