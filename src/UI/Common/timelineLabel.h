//
// Created by nathan on 01/01/2026.
//

#pragma once
#include "imgui.h"
#include "../../SessionData.h"
#include "../../Theme.h"



class TimelineLabel {
    public:
    int totalBars = 30;



    TimelineLabel() = default;

    void create(float timelineLength, float &xScroll, float bars) {
        totalBars = bars;
        if (ImGui::BeginChild("Timeline", ImVec2(0, 20),
            false)) {
            ImGui::SetScrollX(xScroll);
            ImGui::Dummy(ImVec2(timelineLength,0));
            DrawBarLabel();
        }ImGui::EndChild();

    }

    void DrawBarLabel() {
        auto cursorPos = ImGui::GetCursorScreenPos();
        auto drawList = ImGui::GetWindowDrawList();
        const float barWidth = SessionData::instance().getBarWidth();

        for (int bar = 0; bar < totalBars; bar++) {
            char label[16];
            snprintf(label, sizeof(label), "%d", bar);

            drawList->AddText(
                ImVec2((bar * barWidth)+cursorPos.x+2,
                    cursorPos.y),
                    Theme::currentThemeColours.barColourPacked,
                label
            );
            drawList->AddLine(ImVec2(cursorPos.x+bar*barWidth, cursorPos.y),
                ImVec2(cursorPos.x+bar*barWidth,cursorPos.y+20), Theme::currentThemeColours.barColourPacked,1);
        }
    }

};
