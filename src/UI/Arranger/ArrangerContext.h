#pragma once
#include <imgui.h>
#include "../../Singletons/ViewState.h"

struct ArrangerContext {
    ImVec2 cursorPos;
    ImDrawList* drawList;
    float height;
    float scrollX;
    float width;
    int firstVisibleBeat;
    int lastVisibleBeat;
    float barWidth;
    ToolTypes activeTool;
    float relativeX;
    float relativeY;

    ArrangerContext() {
        cursorPos = ImGui::GetCursorScreenPos();
        drawList = ImGui::GetWindowDrawList();
        height = ImGui::GetWindowHeight();
        width = ImGui::GetWindowWidth();
        scrollX = ImGui::GetScrollX();
        activeTool = EDIT;
        barWidth = ViewState::instance().getPixelPerBar(zoomFactor::arranger);
        ImVec2 mousePos = ImGui::GetMousePos();
        relativeX = mousePos.x - cursorPos.x;
        relativeY = mousePos.y - cursorPos.y;
        auto& session = ViewState::instance();
        firstVisibleBeat = scrollX != 0.0f ?
            static_cast<int>(scrollX / session.getPixelPer(Division::QUARTER_NOTE, zoomFactor::arranger)) : 0;
        lastVisibleBeat = static_cast<int>((scrollX + width) / session.getPixelPerBeat(zoomFactor::arranger));
    }
};
