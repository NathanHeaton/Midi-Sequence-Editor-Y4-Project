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
#include "imgui_internal.h"

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

static ImVec2 minusImVec2(ImVec2 a, ImVec2 b) {

    return ImVec2(a.x - b.x, a.y - b.y);
}

// ImGui gradient that also changes alpha ================================================
inline void ShadeVertsLinearGradient(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx,
    ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1)
{
    ImVec2 gradient_extent = minusImVec2(gradient_p1, gradient_p0);
    float gradient_inv_length2 = 1.0f / ImLengthSqr(gradient_extent);
    ImDrawVert* vert_start = draw_list->VtxBuffer.Data + vert_start_idx;
    ImDrawVert* vert_end   = draw_list->VtxBuffer.Data + vert_end_idx;

    const int col0_r = (col0 >> IM_COL32_R_SHIFT) & 0xFF;
    const int col0_g = (col0 >> IM_COL32_G_SHIFT) & 0xFF;
    const int col0_b = (col0 >> IM_COL32_B_SHIFT) & 0xFF;
    const int col0_a = (col0 >> IM_COL32_A_SHIFT) & 0xFF; // <-- add alpha

    const int col_delta_r = ((col1 >> IM_COL32_R_SHIFT) & 0xFF) - col0_r;
    const int col_delta_g = ((col1 >> IM_COL32_G_SHIFT) & 0xFF) - col0_g;
    const int col_delta_b = ((col1 >> IM_COL32_B_SHIFT) & 0xFF) - col0_b;
    const int col_delta_a = ((col1 >> IM_COL32_A_SHIFT) & 0xFF) - col0_a; // <-- add alpha

    for (ImDrawVert* vert = vert_start; vert < vert_end; vert++)
    {
        float d = ImDot(minusImVec2(vert->pos, gradient_p0), gradient_extent);
        float t = ImClamp(d * gradient_inv_length2, 0.0f, 1.0f);

        int r = col0_r + (int)(col_delta_r * t);
        int g = col0_g + (int)(col_delta_g * t);
        int b = col0_b + (int)(col_delta_b * t);
        int a = col0_a + (int)(col_delta_a * t); // <-- interpolate alpha

        vert->col = (r << IM_COL32_R_SHIFT) | (g << IM_COL32_G_SHIFT)
                  | (b << IM_COL32_B_SHIFT) | (a << IM_COL32_A_SHIFT);
    }
}


#endif //MYPROJECT_UTILS_H