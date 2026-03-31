// TimelineContext.h
#pragma once
#include "imgui.h"
#include "../../../Singletons/ViewState.h"
#include "../../../Singletons/ToolManager.h"
#include "../../../Singletons/TimeData.h"

struct TimelineContext {
    ImVec2 cursorPos;
    ImDrawList* drawList;
    float height;
    float scrollX;
    float scrollY;
    float width;
    int firstVisibleSubBeat;
    int lastVisibleSubBeat;
    float barWidth;
    float noteHeight;
    float relativeX;
    float relativeY;
    ToolTypes activeTool;

    TimelineContext() {
        cursorPos = ImGui::GetCursorScreenPos();
        drawList = ImGui::GetWindowDrawList();
        height =ViewState::instance().getNoteHeight()*128 ;
        width = ImGui::GetWindowWidth();
        scrollX = ImGui::GetScrollX();
        scrollY = ImGui::GetScrollY();
        barWidth = 2 * TimeData::instance().timeSignature.getNumerator() * ViewState::instance().getPixelPerBar(zoomFactor::pianoRoll);
        noteHeight = ViewState::instance().getNoteHeight();
        auto& view = ViewState::instance();
        firstVisibleSubBeat = scrollX != 0.0f ?
            static_cast<int>(scrollX /
                (view.getPixelPerBeat(pianoRoll)/ view.getRenderedSubDivisions())) : 0;
        lastVisibleSubBeat = static_cast<int>((scrollX + width)
            / (view.getPixelPerBeat(pianoRoll)/view.getRenderedSubDivisions()));
        ImVec2 mousePos = ImGui::GetMousePos();
        relativeX = mousePos.x - cursorPos.x;
        relativeY = mousePos.y - cursorPos.y;
        activeTool = ToolManager::instance().getActiveNoteTool();
    }
};