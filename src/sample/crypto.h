#include <vector>

void handleErrors();
std::vector<unsigned char> decrypt(const unsigned char* message, int messageLength, const unsigned char* key, const unsigned char* iv);
