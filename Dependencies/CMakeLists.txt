# Build ImGui library
add_library(imgui STATIC
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

# Link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(imgui PUBLIC OpenGL::GL)

target_compile_definitions(imgui PUBLIC
        IMGUI_IMPL_OPENGL_LOADER_CUSTOM=0
)

# Add the JUCE-ImGui bridge module
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR/imgui_juce/modules/imgui_impl_juce)