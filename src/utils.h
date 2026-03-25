//
// Created by idad on 20/10/2025.
//

#ifndef MYPROJECT_UTILS_H
#define MYPROJECT_UTILS_H
#include <charconv>
#include <format>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>

#include "imgui.h"

// returns a slice of a vector
template <typename T>
std::vector<T> vector_slice(const std::vector<T>& a, int start_point, int end_point) {

    if (start_point < 0 || end_point >= static_cast<int>(a.size()) || start_point > end_point) {
        throw std::out_of_range("Invalid slice range");
    }
    return std::vector<T>(a.begin() + start_point, a.begin() + end_point + 1);
}

// returns bytes in a hex string
template <typename T>
std::string vector_to_HexString(std::vector<T> a) {
    std::string s;
    for( int i = 0; i < a.size(); i++ ) {
        s = s + std::format("{:X}",a.at(i));
    }

    return s;
}

// probably refactor this
template <typename T>
int vector_bytes_to_int(std::vector<T> a) {
    int value = 0;
    int increment = 0;
    std::vector<uint8_t> singleHexDigits;
    for (uint8_t byte : a) {
        uint8_t high = (byte >> 4) & 0x0F; // upper 4 bits
        uint8_t low  = byte & 0x0F;        // lower 4 bits
        singleHexDigits.push_back(high);
        singleHexDigits.push_back(low);
    }

    for(auto i = singleHexDigits.size()-1; i > 0; i -= 1) {
        value = value + (singleHexDigits.at(i) * pow(16,increment));
        increment++;
    }
    return value;
}

static ImVec2 addImVec2(ImVec2 a, ImVec2 b) {
    return ImVec2(a.x + b.x, a.y + b.y);
}


#endif //MYPROJECT_UTILS_H