
//
// Created by nathan on 24/11/2025.
//
#pragma once

#include "../../../Theme.h"
#include "../../../Singletons/ViewState.h"
#include "../../../Singletons/PatternManager.h"
#include "../../../Singletons/ToolManager.h"

class VelocityLabels {
public:

    VelocityLabels() = default;
    ViewState* view_state = &ViewState::instance();
    bool bg_tone = false;

    void create(float height) {
        if (ImGui::BeginChild("velocity Labels", ImVec2(ViewState::instance().getWhiteSize().x, 0),
            ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
            ImVec2 textPos = ImVec2(0, 0);
            // printf("width of velocity labels: {%f}\n", ImGui::GetContentRegionAvail().y);
            char octaveStr[3];
            octaveStr[0] = 'c';
            octaveStr[1] = '0';
            octaveStr[2] = '\0';
            ImGui::GetWindowDrawList()->AddRect(ImGui::GetCursorPos(),
                ImGui::GetContentRegionAvail(),Theme::currentThemeColours.accentPacked
                );
            ImGui::GetWindowDrawList()->AddText(
                textPos,
                Theme::currentThemeColours.backgroundPacked,
                octaveStr
            );
            }
        ImGui::EndChild();
    }
};