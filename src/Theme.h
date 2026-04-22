//
// Created by nathan on 16/12/2025.
//

#include <imgui.h>

#ifndef MYPROJECT_THEME_H
#define MYPROJECT_THEME_H

struct Gradient
{
    ImVec2 p0;
    ImVec2 p1;
    ImU32 col0;
    ImU32 col1;
};


namespace Theme
{
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
        ImVec4 barColour;
        ImVec4 beatColour;
        ImVec4 subBeatColour;

        ImVec4 textPri;
        ImVec4 textSce;

        ImU32 backgroundPacked;
        ImU32 backgroundAltPacked;

        ImU32 barColourPacked;
        ImU32 beatColourPacked;
        ImU32 subBeatColourPacked;
        ImU32 accentPacked;
        ImU32 selectionPacked;
        Gradient patternClip;

    };


    inline const  ColourScheme DarkBColours = {
        ImVec4(9.0f / 255.0f, 16.0f / 255.0f, 28.0f / 255.0f, 1.0f),
        ImVec4(16.0f / 255.0f, 24.0f / 255.0f, 38.0f / 255.0f, 1.0f),
        ImVec4(187.0 / 255.0f, 196.0f / 255.0f, 213.0f / 255.0f, 1.0f),
        ImVec4(243.0/255.0f, 180.0f/255.0f, 64.0f/255.0f, 1.0f),

        ImVec4(132.0/255.0f, 161.0f/255.0f, 215.0f/255.0f, 1.0f),
        ImVec4(70.0/255.0f, 87.0f/255.0f, 118.0f/255.0f, 1.0f),
        ImVec4(37.0/255.0f, 47.0f/255.0f, 64.0f/255.0f, 1.0f),

        ImVec4(1.0f,1.0f, 1.0f, 1.0f),
        ImVec4(9.0/255.0f, 16.0f/255.0f, 28.0f/255.0f, 1.0f),

        IM_COL32(9,16,28,255),
        IM_COL32(16,24,38,255),
        IM_COL32(132,161,215,255),
        IM_COL32(70,87,118,255),
        IM_COL32(37,47,64,255),
        IM_COL32(243,180,64,255),
        IM_COL32(243,188,64,49),

        Gradient(ImVec2(1,1),
            ImVec2(1,4),
            IM_COL32(9,16,29,255),
            IM_COL32(9,16,29,123))

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

    inline ColourScheme cTheme = DarkBColours;

    inline void applyTheme() {
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_WindowBg] = cTheme.background;
        style.Colors[ImGuiCol_FrameBg] = cTheme.backgroundAlt;
        style.Colors[ImGuiCol_Border] = cTheme.outline;
        style.Colors[ImGuiCol_Button] =  ImVec4(0,0,0,0);
        style.Colors[ImGuiCol_ButtonHovered] = cTheme.backgroundAlt;
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0,0,0,0);
        style.Colors[ImGuiCol_Separator] = cTheme.outline;
    }

    inline void changeTheme(ThemeType theme) {
        switch (theme) {
            case ThemeType::Dark_Blue:
                cTheme = DarkBColours;
            case ThemeType::Dark_Orange:
                cTheme = DarkBColours;
            case ThemeType::Light_Orange:
                cTheme = DarkBColours;
            case ThemeType::Light_Blue:
                cTheme = LightOColours;
        }
    }

    inline ImFont* monoSmall;
    inline ImFont* textMedium;
    inline ImFont* textSmall;
    inline ImFont* windowTitle;
    inline ImFont* trackTitle;
    inline ImFont* textHeading;

    inline float borderSize = 1.0f;
    // In Theme.h or a ThemeApplier.cpp
    static void setImGuiStyle() {
        ImGuiIO& io = ImGui::GetIO();
        textSmall  = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Regular.ttf", 14.0f);
        textMedium  = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Regular.ttf", 16.0f);
        windowTitle  = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Regular.ttf", 24.0f);
        trackTitle  = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Medium.ttf", 24.0f);
        textHeading  = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Regular.ttf", 20.0f);

        monoSmall  = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto_Mono/RobotoMono-VariableFont_wght.ttf", 16.0f);
        auto* s = &ImGui::GetStyle();

        s->WindowPadding     = ImVec2(8, 8);
        s->FramePadding      = ImVec2(12, 8);
        s->ItemSpacing       = ImVec2(4, 4);
        s->ItemInnerSpacing  = ImVec2(2, 2);
        s->ScrollbarSize     = 20.0f;

    }

}
#endif //MYPROJECT_THEME_H