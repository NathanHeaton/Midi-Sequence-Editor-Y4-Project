//
// Created by nathan on 16/12/2025.
//

#include <imgui.h>

#ifndef MYPROJECT_THEME_H
#define MYPROJECT_THEME_H


namespace Theme{

    enum class ThemeType{
        Dark_Blue,
        Dark_Orange,
        Light_Orange,
        Light_Blue,

    };

    struct ColourScheme {
        ImVec4 background;
        ImVec4 backgroundAlt;

        ImVec4 outline;
        ImVec4 accent;
        ImVec4 beatColour;
        ImVec4 barColour;

        ImVec4 textPri;
        ImVec4 textSce;

        ImU32 backgroundPacked;
        ImU32 backgroundAltPacked;
        ImU32 beatColourPacked;
        ImU32 barColourPacked;

        // constexpr ColourScheme() {
        //     backgroundPacked(ImU32)
        // }
    };

    inline const  ColourScheme DarkBColours = {
        ImVec4(9.0f / 255.0f, 16.0f / 255.0f, 28.0f / 255.0f, 1.0f),
        ImVec4(16.0f / 255.0f, 24.0f / 255.0f, 38.0f / 255.0f, 1.0f),
        ImVec4(187.0 / 255.0f, 196.0f / 255.0f, 213.0f / 255.0f, 1.0f),
        ImVec4(243.0/255.0f, 180.0f/255.0f, 64.0f/255.0f, 1.0f),
        ImVec4(37.0/255.0f, 47.0f/255.0f, 64.0f/255.0f, 1.0f),
        ImVec4(93.0/255.0f, 106.0f/255.0f, 131.0f/255.0f, 1.0f),
        ImVec4(1.0f,1.0f, 1.0f, 1.0f),
        ImVec4(9.0/255.0f, 16.0f/255.0f, 28.0f/255.0f, 1.0f),
        IM_COL32(9,16,28,255),
        IM_COL32(16,24,38,255),
        IM_COL32(37,47,64,255),
        IM_COL32(93,106,131,255)

    };

    inline constexpr ColourScheme LightOColours = {
        ImVec4(9.0f / 255.0f, 16.0f / 255.0f, 28.0f / 255.0f, 1.0f),
        ImVec4(16.0f / 255.0f, 24.0f / 255.0f, 38.0f / 255.0f, 1.0f),
        ImVec4(187.0 / 255.0f, 196.0f / 255.0f, 213.0f / 255.0f, 1.0f),
        ImVec4(243.0/255.0f, 180.0f/255.0f, 64.0f/255.0f, 1.0f),
        ImVec4(93.0/255.0f, 106.0f/255.0f, 131.0f/255.0f, 1.0f),
        ImVec4(37.0/255.0f, 47.0f/255.0f, 64.0f/255.0f, 1.0f),
        ImVec4(1.0f,1.0f, 1.0f, 1.0f),
        ImVec4(9.0/255.0f, 16.0f/255.0f, 28.0f/255.0f, 1.0f)
    };

    inline ColourScheme currentThemeColours = DarkBColours;

    inline void applyTheme() {
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_WindowBg] = currentThemeColours.background;
        style.Colors[ImGuiCol_FrameBg] = currentThemeColours.backgroundAlt;
        style.Colors[ImGuiCol_Border] = currentThemeColours.outline;

    }

    inline void changeTheme(ThemeType theme) {
        switch (theme) {
            case ThemeType::Dark_Blue:
                currentThemeColours = DarkBColours;
            case ThemeType::Dark_Orange:
                currentThemeColours = DarkBColours;
            case ThemeType::Light_Orange:
                currentThemeColours = DarkBColours;
            case ThemeType::Light_Blue:
                currentThemeColours = LightOColours;
        }
    }

}
#endif //MYPROJECT_THEME_H