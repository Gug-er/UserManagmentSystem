#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include "hash.h"

std::string hash_password(std::string password)// FNV-1a 
{
    unsigned long long hash = 1469598103934665603ULL;

    for(char c : password)
    {
    hash ^= (unsigned long long)c;
    }
    hash *= 1099511628211ULL;

    std::stringstream ss;
    ss << std::hex << hash;
    return ss.str();
}