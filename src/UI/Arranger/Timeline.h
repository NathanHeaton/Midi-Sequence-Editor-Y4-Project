//
// Created by nathan on 09/12/2025.
//
#pragma once
#include "imgui.h"
#include "../../SessionData.h"

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
            int barFirst = firstVisableBar(scrollX);
            int barLast = lastVisableBar(scrollX);
            auto barsScrolled = firstVisableBar(scrollX)* s->getPixelPerBeat();

            draw_list->AddCircle(ImVec2(50,50),30, IM_COL32(255, 255, 255, 255));

            for (auto i = barFirst; i < barLast ; i++) {
                float beatPos = i * s->getPixelPerBeat();
                draw_list->AddLine(ImVec2(beatPos,0),
                    ImVec2(beatPos, height), IM_COL32(255, 255, 255, 255),
                    1);
            }

            ImGui::Dummy(ImVec2(10000, 0));

        }ImGui::EndChild();

    }

    int firstVisableBar(float scrollX) {
        auto beats = s->timeSignature.numerator;
        int firstVisible;
        if ( scrollX == 0) {firstVisible = 0; }
        else{firstVisible = static_cast<int>(scrollX / s->getPixelPerBeat());}
        return firstVisible;
    }

    int lastVisableBar(float scrollX) {
        auto beats = s->timeSignature.numerator;
        int lastVisible;
        if ( scrollX == 0) {lastVisible = 0; }
        else{lastVisible = static_cast<int>(scrollX / s->getPixelPerBeat());}
        return lastVisible;
    }

};
