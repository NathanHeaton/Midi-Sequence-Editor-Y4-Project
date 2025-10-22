//
// Created by idad on 20/10/2025.
//

#ifndef MYPROJECT_UTILS_H
#define MYPROJECT_UTILS_H
#include <charconv>
#include <format>


// returns a slice of a vector
template <typename T>
std::vector<T> vector_slice(std::vector<T> a,int start_point,int end_point ) {
    std::vector<T> b;
    for( int i = start_point; i <= end_point; i++ ) {
        if( static_cast<unsigned int>(a.back()) > start_point + end_point ) {
            throw std::invalid_argument("out of range");
        }
        b.insert(b.begin()+i - start_point,a[i]);
    }
    return b;
}

// returns bytes in a hes string
template <typename T>
juce::String vector_to_HexString(std::vector<T> a) {
    juce::String s;
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

    for(int i = singleHexDigits.size()-1; i >= 0; i -= 1) {
        value = value + (singleHexDigits.at(i) * pow(16,increment));
        increment++;
    }
    return value;
}


#endif //MYPROJECT_UTILS_H