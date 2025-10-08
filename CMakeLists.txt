cmake_minimum_required(VERSION 3.24)
project(myProject VERSION 0.0.1)

set(JUCE_ENABLE_LTO OFF)

add_subdirectory(modules/JUCE)
add_subdirectory(src)

