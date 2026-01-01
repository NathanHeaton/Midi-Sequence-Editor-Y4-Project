//
// Created by nathan on 01/01/2026.
//

#pragma once
#include "imgui.h"
#include "../../SessionData.h"
#include "../../Theme.h"


class TimelineLabel {
    public:
    TimelineLabel() = default;

    void create() {
        if (ImGui::BeginChild("Timeline", ImVec2(0, 10),
            false,
            ImGuiWindowFlags_NoScrollbar)) {
            DrawBarLabel();
            ImGui::Dummy(ImVec2(10000, 0));
            ImGui::Text("labels");
            }
        ImGui::EndChild();

    }

    void DrawBarLabel() {

        auto cursorPos = ImGui::GetCursorPos();
        auto drawList = ImGui::GetWindowDrawList();
        int bars = 30;
        for (int i = 0; i < bars; i++) {
            char label[16];
            int barNumber = i;
            snprintf(label, sizeof(label), "%d", barNumber);
            drawList->AddText(
                ImVec2((bars * (SessionData::instance().getPixelPerBeat() * SessionData::instance().timeSignature.getNumerator()))
                    + cursorPos.x + 2, cursorPos.y),
                Theme::currentThemeColours.barColourPacked,
                label
            );
        }
    }

};
