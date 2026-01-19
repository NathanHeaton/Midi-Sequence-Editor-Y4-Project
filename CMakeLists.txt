cmake_minimum_required(VERSION 3.24)
project(myProject VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Global LTO settings
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO OFF)
set(JUCE_ENABLE_LTO OFF CACHE BOOL "Disable JUCE LTO" FORCE)

# Add JUCE
set(JUCE_DIR "/home/idad/Projects/4th_year_project_stuff/JUCE/install/lib/cmake/JUCE-8.0.10")

#set(JUCE_DIR "../../JUCE/install/lib/cmake/JUCE-8.0.10")

find_package(JUCE CONFIG REQUIRED)

# Build ImGui library with OpenGL3 backend
add_library(imgui STATIC
        Dependencies/imgui/imgui.cpp
        Dependencies/imgui/imgui_demo.cpp
        Dependencies/imgui/imgui_draw.cpp
        Dependencies/imgui/imgui_tables.cpp
        Dependencies/imgui/imgui_widgets.cpp
        Dependencies/imgui/backends/imgui_impl_opengl3.cpp
        src/Audio/MidiPlayer.h
        src/Singletons/PlayBackEngine.h
        src/Audio/AuidoPlayer.h
)

target_include_directories(imgui PUBLIC
        Dependencies/imgui
        Dependencies/imgui/backends
)

# Link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(imgui PUBLIC OpenGL::GL)

add_subdirectory(Dependencies/imgui_juce)

# Source
add_subdirectory(src)

