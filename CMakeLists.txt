cmake_minimum_required(VERSION 3.24)
project(myProject VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Global LTO settings
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO OFF)
set(JUCE_ENABLE_LTO OFF CACHE BOOL "Disable JUCE LTO" FORCE)

# Add JUCE and your source
set(JUCE_DIR "/home/idad/Projects/4th_year_project_stuff/JUCE/install/lib/cmake/JUCE-8.0.10")
##set(JUCE_DIR "/home/nathan/JUCE/install/lib/cmake/JUCE-8.0.10")
find_package(JUCE CONFIG REQUIRED)

add_subdirectory(src)

# --- Force-disable LTO for all JUCE-related targets ---
get_property(allTargets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
foreach(tgt ${allTargets})
    set_target_properties(${tgt} PROPERTIES INTERPROCEDURAL_OPTIMIZATION FALSE)
endforeach()

