//
// Created by idad on 20/10/2025.
//

#ifndef MYPROJECT_UTILS_H
#define MYPROJECT_UTILS_H
#include <charconv>
#include <format>



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

template <typename T>
juce::String vector_to_HexString(std::vector<T> a) {
    juce::String s;
    for( int i = 0; i < a.size(); i++ ) {
        s = s + std::format("{:X}",a.at(i));
    }

    return s;
}

#endif //MYPROJECT_UTILS_H