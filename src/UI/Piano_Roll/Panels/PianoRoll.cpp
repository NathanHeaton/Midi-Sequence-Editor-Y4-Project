#include <algorithm>
#include "PianoRollComponent.h"

//
// Created by nathan on 16/02/2026.
//
void PianoRollComponent::HandleMouseInput(const TimelineContext& ctx) {
    if (!ImGui::IsWindowHovered()) {
        return;
    }
    switch (ctx.activeTool) {
        case MOVE:
            break;
        case DELETE:
            break;
        case SNIP:
            break;
        case SELECT:
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                ToolManager::instance().setSelectionPoint1(ImVec2{ctx.relativeX, ctx.relativeY});
            }
            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                ToolManager::instance().setSelectionPoint2(ImVec2{ctx.relativeX, ctx.relativeY});
                PatternManager::instance().setSelection(ToolManager::instance().getSelectionPoints());
            }
            break;
        case EDIT:
            edit(ctx);
            break;

    }
}

void PianoRollComponent::edit(const TimelineContext& ctx)
{
    int noteTime = static_cast<int>(ctx.relativeX / view_state->getPixelPerBeat(pianoRoll) *view_state->getSnappedSubDivisions());
    int absoluteTime = static_cast<int>(ctx.relativeX / view_state->getPixelPerBeat(pianoRoll) *TimeData::PPQ);
    int pitch = static_cast<int>(ctx.relativeY / ctx.noteHeight);
    pitch = std::clamp(pitch, 0, 127);
    pitch = 127 - pitch;
    NoteCoordinate hoverCoordinate(pitch, static_cast<uint32_t>(absoluteTime));
    auto noteHoverState = PatternManager::instance().getNoteHoverState(hoverCoordinate);
    int snappedTime = noteTime * (static_cast<float>(TimeData::PPQ)/view_state->getSnappedSubDivisions());
    NoteCoordinate snappedCoordinate(pitch, static_cast<uint32_t>(snappedTime));

    if (moveOperation.isMovingNote)
    {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            moveOperation.updateMovingNotesPosition(snappedCoordinate.pitch,snappedCoordinate.absoluteTime);
        }
        else
        {
            if (moveOperation.movingNotes.size() == 1 )
            {
                auto note = moveOperation.movingNotes.at(0);

                NoteCoordinate newPos(note.pitch +moveOperation.newDeltaPitch, note.absoluteTime+moveOperation.newDeltaTime);

                PatternManager::instance().moveNoteEvent(note.ID,
                    newPos,
                    note.endAbsoluteTime+ moveOperation.newDeltaTime);
            }
            moveOperation.clearNotes();

            PatternManager::instance().showAllEvents();
        }
    }


    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        if (noteHoverState == NoteCenterHover) {
            std::cout << "Hover"<< std::endl;
            PatternManager::instance().hideNoteEvent(hoverCoordinate);
            moveNote(ctx, snappedCoordinate);
        }
        else if (noteHoverState == NoteEdgeHover) {
            std::cout << "edge Hover"<< std::endl;
        }
        else
        {
            sendNewNote(snappedCoordinate);
        }
    }
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsMouseDown(ImGuiMouseButton_Right)) {

        PatternManager::instance().removeNoteFromPattern(hoverCoordinate);
    }
}

void PianoRollComponent::sendNewNote(NoteCoordinate snappedCoordinate) {
    int duration = TimeData::PPQ;
    PatternManager::instance().addNoteToPattern(snappedCoordinate.pitch,
        static_cast<signed>(snappedCoordinate.absoluteTime),  duration);
}

void PianoRollComponent::moveNote(const TimelineContext& ctx, NoteCoordinate snappedCoordinate)
{
    auto notePair = PatternManager::instance().getNoteEventPairFromCoordinate(snappedCoordinate);

    const auto& noteData = PatternManager::instance().getCurrentPattern().m_events;
    const auto& onNote =noteData.at(notePair.onIndex);
    MovingNoteSnapshot noteSnapshot(
        onNote.getID(),
        onNote.getAbsoluteTime(),
        noteData.at(notePair.offIndex).getAbsoluteTime(),
        noteData.at(notePair.offIndex).getAbsoluteTime() - onNote.getAbsoluteTime(),
        onNote.getPitch()
        );
    moveOperation.originalInputCoordinate = snappedCoordinate;
    moveOperation.addNotes(noteSnapshot);
}


void PianoRollComponent::renderPattern(const TimelineContext& ctx) const{
    auto& pattern = PatternManager::instance().getCurrentPattern();
    auto& noteData = pattern.m_events;

    for (auto noteIndices : pattern.m_noteEvents) {
        auto onIndex = noteData.at(noteIndices.onIndex);
        if (pattern.m_hiddenNoteIDs.contains(onIndex.getID()))
        {
            continue;
        }
        auto offIndex = noteData.at(noteIndices.offIndex);

        float startDelta =0;
        if (onIndex.m_absoluteTime != 0){startDelta =
            static_cast<float>(onIndex.m_absoluteTime) / TimeData::PPQ;}

        float endDelta=0;
        if (offIndex.m_absoluteTime != 0){endDelta =
            static_cast<float>(offIndex.m_absoluteTime) / TimeData::PPQ;}

        float startPixel = view_state->getPixelPerBeat(zoomFactor::pianoRoll) * startDelta;
        float endPixel = view_state->getPixelPerBeat(zoomFactor::pianoRoll) * endDelta ;

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

void PianoRollComponent::renderMovingNotes(const TimelineContext& ctx)
{
    for (auto note: moveOperation.movingNotes)
    {
        std::cout<<"got moving note data"<<std::endl;
        auto onTime = note.absoluteTime + moveOperation.newDeltaTime;
        auto offTime = note.endAbsoluteTime + moveOperation.newDeltaTime;
        auto pitch = note.pitch + moveOperation.newDeltaPitch;


        float startDelta =0;
        if (onTime != 0){startDelta =
            static_cast<float>(onTime) / TimeData::PPQ;}

        float endDelta=0;
        if (offTime != 0){endDelta =
            static_cast<float>(offTime) / TimeData::PPQ;}

        float startPixel = view_state->getPixelPerBeat(zoomFactor::pianoRoll) * startDelta;

        float endPixel = view_state->getPixelPerBeat(zoomFactor::pianoRoll) * endDelta ;

        float xStart = startPixel + ctx.cursorPos.x;
        float xEnd = endPixel + ctx.cursorPos.x;
        float yStart = ctx.cursorPos.y+ ( ctx.height - ((pitch+1)*ctx.noteHeight));

        ctx.drawList->AddRectFilled(
    ImVec2(xStart, yStart),
    ImVec2(xEnd,yStart + ctx.noteHeight),
        Theme::currentThemeColours.beatColourPacked, 3.0f);
    }
}

void PianoRollComponent::DrawToolEffects(const TimelineContext& ctx) {
    if (ctx.activeTool == SELECT) {
        if (ToolManager::instance().isBoxSelecting()) {
            ImVec2 p1 = ToolManager::instance().getSelectionPoint1();
            ImVec2 p1Box = ImVec2(p1.x + ctx.cursorPos.x, p1.y + ctx.cursorPos.y);
            ImVec2 p2Box = ImVec2(
                ctx.relativeX + ctx.cursorPos.x,
                ctx.relativeY + ctx.cursorPos.y
            );
            ctx.drawList->AddRectFilled(
                p1Box,
                p2Box,
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
        else if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
            PatternManager::instance().pitchShiftSelection(+12);
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
            PatternManager::instance().pitchShiftSelection(-120);
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
            PatternManager::instance().pitchShiftSelection(+120);
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_Z)) {

        }
    }
    if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {
        PatternManager::instance().deleteSelection();
    }

}