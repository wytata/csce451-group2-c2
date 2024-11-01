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

    printf("Input: ");

    std::string input;
    std::cin >> input;

    if (atoi(input.c_str()) == key) printf("YOU KNOW THE KEY\n");
    else printf("You input: %s\n", input.c_str());


    return 0;
}