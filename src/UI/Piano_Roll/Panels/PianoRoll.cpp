#include <algorithm>
#include "PianoRollComponent.h"

//
// Created by nathan on 16/02/2026.
//
void PianoRollComponent::HandleMouseInput(const TimelineContext& ctx) {
    bool isHovered = ImGui::IsWindowHovered();

    switch (ctx.activeTool) {
        case EDIT:
            if (isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                int beat = static_cast<int>(ctx.relativeX / s->getPixelPerBeat(zoomFactor::pianoRoll));
                int absoluteTime = beat * s->getPPQ();
                int pitch = static_cast<int>(ctx.relativeY / ctx.noteHeight);
                pitch = std::clamp(pitch, 0, 127);
                pitch = 127 - pitch;
                int duration = s->getPPQ();

                PatternManager::instance().addNoteToPattern(pitch,  absoluteTime,  duration);
            }
            if (isHovered && (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsMouseDown(ImGuiMouseButton_Right))) {
                float beat = ctx.relativeX / s->getPixelPerBeat(zoomFactor::pianoRoll);
                int absoluteTime = beat * s->getPPQ();
                int pitch = static_cast<int>(ctx.relativeY / ctx.noteHeight);
                pitch = std::clamp(pitch, 0, 127);
                pitch = 127 - pitch;

                noteCoordinate note_coordinate(pitch, absoluteTime);
                PatternManager::instance().removeNoteFromPattern(note_coordinate);

            }
            break;
        case SELECT:
            if (isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                ToolManager::instance().setSelectionPoint1(ImVec2{ctx.relativeX, ctx.relativeY});
            }
            if (isHovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                ToolManager::instance().setSelectionPoint2(ImVec2{ctx.relativeX, ctx.relativeY});
                PatternManager::instance().setSelection(ToolManager::instance().getSelectionPoints());
            }
            if (!isHovered && ToolManager::instance().isBoxSelecting()) {
                ToolManager::instance().setSelectionPoint2(ImVec2{ctx.relativeX, ctx.relativeY});
            }
            break;
    }
}


void PianoRollComponent::renderPattern(const TimelineContext& ctx) {
    auto& pattern = PatternManager::instance().getCurrentPattern();
    auto& noteData = pattern.m_events;

    for (auto noteIndices : pattern.m_noteEvents) {
        auto onIndex = noteData.at(noteIndices.onIndex);
        auto offIndex = noteData.at(noteIndices.offIndex);

        float startDelta =0;
        if (onIndex.m_absoluteTime != 0){startDelta =
            static_cast<float>(onIndex.m_absoluteTime) / s->getPPQ();}

        float endDelta=0;
        if (offIndex.m_absoluteTime != 0){endDelta =
            static_cast<float>(offIndex.m_absoluteTime) / s->getPPQ();}

        float startPixel = s->getPixelPerBeat(zoomFactor::pianoRoll) * startDelta;
        float endPixel = s->getPixelPerBeat(zoomFactor::pianoRoll) * endDelta ;
        auto pitch =  noteData.at(noteIndices.onIndex).getPitch();

        float yStart = ctx.cursorPos.y + ( ctx.height - ((noteData.at(noteIndices.onIndex).getPitch()+1)*ctx.noteHeight));
        float xStart = ctx.cursorPos.x + startPixel;
        float xEnd =  ctx.cursorPos.x + endPixel;
        float yEnd = yStart + ctx.noteHeight;

        auto colour = Theme::currentThemeColours.barColourPacked;
        for (auto id : pattern.m_selectedNoteIDs) {
            if (noteData.at(noteIndices.onIndex).getID() == id) {
                colour = Theme::currentThemeColours.beatColourPacked;
                break;
            }
        }
        ctx.drawList->AddRectFilled(
            ImVec2(xStart, yStart),
            ImVec2(xEnd,yEnd),
                colour, 3.0f);
    }
}

void PianoRollComponent::DrawToolEffects(const TimelineContext& ctx) {
    if (ctx.activeTool == SELECT) {
        if (ToolManager::instance().isBoxSelecting()) {
            ImVec2 p1 = ToolManager::instance().getSelectionPoint1();
            ImVec2 p1Screen = ImVec2(p1.x + ctx.cursorPos.x, p1.y + ctx.cursorPos.y);
            ImVec2 p2Screen = ImVec2(
                ctx.relativeX + ctx.cursorPos.x,
                ctx.relativeY + ctx.cursorPos.y
            );

            ctx.drawList->AddRectFilled(
                p1Screen,
                p2Screen,
                Theme::currentThemeColours.barColourPacked, 0.0f);
        }
    }
}

void PianoRollComponent::HandleKeyboardInput(const TimelineContext& ctx) {
    if (ImGui::IsKeyDown(ImGuiMod_Shift)) {
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
            PatternManager::instance().pitchShiftSelection(-1);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
            PatternManager::instance().pitchShiftSelection(+1);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
            PatternManager::instance().timeShiftSelection(-100);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
            PatternManager::instance().timeShiftSelection(+100);
        }
    }
    if (ImGui::IsKeyDown(ImGuiMod_Ctrl)) {
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
            PatternManager::instance().pitchShiftSelection(-12);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
            PatternManager::instance().pitchShiftSelection(+12);
        }
    }
}