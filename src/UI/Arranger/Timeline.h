//
// Created by nathan on 09/12/2025.
//
#pragma once
#include "imgui.h"
#include "../../SessionData.h"
#include  "../../Theme.h"

class Timeline {
public:
    Timeline() = default;
    SessionData* s = &SessionData::instance();

    void createTimeline() {


        if (ImGui::BeginChild("Timeline", ImVec2(0, 100 * s->getTrackAmount()), true,
            ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {
            ImVec2 cursorPos = ImGui::GetCursorScreenPos();
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            float height = ImGui::GetWindowHeight();
            float scrollX = ImGui::GetScrollX();
            float width = ImGui::GetWindowWidth();

            int firstVisibleBeat = 0;

            if (scrollX != 0.0) firstVisibleBeat = static_cast<int>(scrollX / s->getPixelPerBeat());

            int lastVisibleBeat = static_cast<int>(scrollX+ width / s->getPixelPerBeat());

            for (auto i = firstVisibleBeat; i <= lastVisibleBeat ; i++) {
                float beatPos = cursorPos.x + i * s->getPixelPerBeat();

                draw_list->AddLine(ImVec2(beatPos,cursorPos.y),
                    ImVec2(beatPos, cursorPos.y + height), Theme::currentThemeColours.beatColourPacked,
                    1);
            }

            ImGui::Dummy(ImVec2(10000, 0));

        }ImGui::EndChild();

    }
};
