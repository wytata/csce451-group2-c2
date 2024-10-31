#ifndef CAESAR_H
#define CAESAR_H

#include <string>

std::string encrypt(std::string input, char key);
std::string decrypt(std::string input, char key);

#endif // !CAESAR_H
