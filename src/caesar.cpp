#include "caesar.h"

std::string encrypt(std::string input, char key) {
    std::string result;
    if (key > 26) key = key % 26;
    for (int i = 0; i < input.size(); i++) { 
        char next;
        if (input.at(i) >= 'a' && input.at(i) <= 'z') {
            next = input.at(i) + key; 
            if (input.at(i) + key > 'z') {
                next = input.at(i) + key - 'z' + 'a' - 1;
            }
        } else if (input.at(i) >= 'A' && input.at(i) <= 'Z') {
            next = input.at(i) + key; 
            if (input.at(i) + key > 'Z') {
                next = input.at(i) + key - 'Z' + 'A' - 1;
            }
        } else {
            next = input.at(i); 
        }
        result.push_back(next);
    }
    return result;
}

std::string decrypt(std::string input, char key) {
    std::string result;
    for (int i = 0; i < input.size(); i++) { 
        char next;
        if (input.at(i) >= 'a' && input.at(i) <= 'z') {
            next = input.at(i) - key; 
            if (next < 'a') {
                next = next + 'z' - 'a' + 1;
            }
        } else if (input.at(i) >= 'A' && input.at(i) <= 'Z') {
            next = input.at(i) - key; 
            if (next < 'A') {
                next = next + 'Z' - 'A' + 1;
            }
        } else {
            next = input.at(i); 
        }
        result.push_back(next);
    }
    return result;

}



