
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
    std::string numLabels[3]{"127","64","1"};

    void create() {
        if (ImGui::BeginChild("velocity Labels", ImVec2(ViewState::instance().getWhiteSize().x, 0),
            ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
            auto cursorPos = ImGui::GetCursorScreenPos();
            auto height = ImGui::GetContentRegionAvail().y;
            auto increment = height / numLabels->length();
            for (auto i{0u}; i < numLabels->length(); ++i) {
                ImVec2 textPos = ImVec2(cursorPos.x+ 10, cursorPos.y + (increment * i));

                auto labelC = numLabels[i].c_str();
                ImGui::GetWindowDrawList()->AddText(
                    textPos,
                    Theme::currentThemeColours.barColourPacked,
                    labelC
                );
            }
        } ImGui::EndChild();
    }
};