//
// Created by nathan on 01/01/2026.
//

#pragma once
#include "imgui.h"
#include "../../Singletons/ViewState.h"
#include "../../Singletons/TimeData.h"
#include "../../Singletons/PlayBackManager.h"
#include "../../Theme.h"



class TimelineLabel {
    public:
    int totalBars = 30;
    float barWidth;
    float m_zoomFactor;
    float height = 20.0f;


    TimelineLabel() = default;

    void create(float timelineLength, float &xScroll, int bars, float t_zoomFactor) {
        totalBars = bars;
        m_zoomFactor =t_zoomFactor;
        barWidth = ViewState::instance().getPixelPerBar(m_zoomFactor);
        if (ImGui::BeginChild("Timeline", ImVec2(0, height),
            false)) {
            ImGui::SetScrollX(xScroll);
            ImGui::Dummy(ImVec2(timelineLength,0));
            DrawBarLabel();
            DrawPlayHead();
            HandleMouse();
        }ImGui::EndChild();

    }

    void DrawBarLabel() {
        auto cursorPos = ImGui::GetCursorScreenPos();
        auto drawList = ImGui::GetWindowDrawList();
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

    void DrawPlayHead() {
        auto cursorPos = ImGui::GetCursorScreenPos();
        auto drawList = ImGui::GetWindowDrawList();

        float xPos = cursorPos.x + ViewState::instance().getPixelPerBeat(pianoRoll)*
            (PlayBackManager::instance().getPlayheadPositionTicks()/TimeData::instance().PPQ);

        drawList->AddLine(ImVec2(xPos,cursorPos.y+ 0),
        ImVec2(xPos,cursorPos.y+ height),
        Theme::currentThemeColours.barColourPacked, 15
            );
    }
    bool firstMouseUp = false;
    bool mouseDown = false;
    void HandleMouse() {
        auto cursorPos = ImGui::GetCursorScreenPos();
        auto mousePos = ImGui::GetMousePos();


        if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
            PlayBackManager::instance().setPlaying(false);
            unsigned int ticks = TimeData::instance().PPQ *((mousePos.x - cursorPos.x)/ ViewState::instance().getPixelPerBeat(pianoRoll));
            PlayBackManager::instance().setPlayHeadPositionTicks(ticks);
            mouseDown = true;
        }
        else if (mouseDown){firstMouseUp = true; mouseDown = false;}

        if(firstMouseUp) {
            PlayBackManager::instance().togglePlay();
            firstMouseUp = false;
        }
    }

};
