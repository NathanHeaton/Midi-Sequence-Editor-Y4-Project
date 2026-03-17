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

    if (moveOperation.isMovingNote)    {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left))        {
            moveOperation.updateMovingNotesPosition(snappedCoordinate.pitch,snappedCoordinate.absoluteTime);
        }
        else{
            if (moveOperation.movingNotes.size() == 1 ){
                auto note = moveOperation.movingNotes.at(0);
                NoteCoordinate newPos(note.pitch +moveOperation.newDeltaPitch, note.absoluteTime+moveOperation.newDeltaTime);
                PatternManager::instance().moveNoteEvent(note.ID,newPos);
            }
            else {
                auto note = moveOperation.movingNotes.at(0);
                NoteCoordinate newPos(moveOperation.newDeltaPitch, moveOperation.newDeltaTime);
                PatternManager::instance().moveSelection(newPos);
            }

            moveOperation.clearNotes();
            PatternManager::instance().showAllEvents();
        }
    }
    else if (stretchOperation.isStretchingNote) {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left))        {
            stretchOperation.updateStretchDelta(snappedCoordinate.absoluteTime);
        }
        else{
            if (stretchOperation.stretchingNotes.size() == 1 ){
                auto note = stretchOperation.stretchingNotes.at(0);
                PatternManager::instance().stretchNoteEvent(note.ID, stretchOperation.newEndDelta);
            }
            stretchOperation.clearNotes();
            PatternManager::instance().showAllEvents();
        }
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        if (noteHoverState == NoteCenterHover) {
            moveNote(snappedCoordinate);
        }
        else if (noteHoverState == NoteEdgeHover) {
            PatternManager::instance().hideNoteEvent(hoverCoordinate);
            stretchNote(snappedCoordinate);
        }
        else{
            sendNewNote(snappedCoordinate);
        }
    }
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        PatternManager::instance().removeNoteFromPattern(hoverCoordinate);
    }
}

void PianoRollComponent::sendNewNote(NoteCoordinate snappedCoordinate) {
    PatternManager::instance().addNoteToPattern(snappedCoordinate.pitch,
        static_cast<signed>(snappedCoordinate.absoluteTime),  ToolManager::instance().getLastNoteDuration());
}

void PianoRollComponent::moveNote(NoteCoordinate snappedCoordinate){
    auto* pattern = &PatternManager::instance().getCurrentPattern();
    std::vector<NoteSnapshot> noteSnapshots;
    if (pattern->m_selectedNoteIDs.size() > 0) {
        for (uint32_t m_selected_note_i_d : pattern->m_selectedNoteIDs) {
            noteSnapshots.push_back(createNoteSnapShotBasedOnID(m_selected_note_i_d));
        }
    }
    else {
        noteSnapshots.push_back(createNoteSnapShot(snappedCoordinate));
    }

    moveOperation.originalInputCoordinate = snappedCoordinate;
    moveOperation.addNotes(noteSnapshots);
}

NoteSnapshot PianoRollComponent::createNoteSnapShotBasedOnID(uint32_t onID) {
    auto* pattern = &PatternManager::instance().getCurrentPattern();
    auto noteIds = *pattern->getEventIDPairFromOnID(onID);
    auto onNote = pattern->getMidiEventByID_ptr(noteIds.onID);
    auto offNote = pattern->getMidiEventByID_ptr(noteIds.offID);

    PatternManager::instance().hideNoteEventByID(onID);
    return NoteSnapshot(
        noteIds.onID,
        onNote->getAbsoluteTime(),
        offNote->getAbsoluteTime(),
        offNote->getAbsoluteTime() - onNote->getAbsoluteTime(),
        onNote->getPitch()
        );
}

NoteSnapshot PianoRollComponent::createNoteSnapShot(NoteCoordinate snappedCoordinate) {
    auto noteIds = PatternManager::instance().getNoteEventPairFromCoordinate(snappedCoordinate);
    auto* pattern = &PatternManager::instance().getCurrentPattern();
    auto onNote = pattern->getMidiEventByID_ptr(noteIds.onID);
    auto offNote = pattern->getMidiEventByID_ptr(noteIds.offID);

    PatternManager::instance().hideNoteEventByID(noteIds.onID);
    return NoteSnapshot(
        noteIds.onID,
        onNote->getAbsoluteTime(),
        offNote->getAbsoluteTime(),
        offNote->getAbsoluteTime() - onNote->getAbsoluteTime(),
        onNote->getPitch()
        );
}

void PianoRollComponent::stretchNote(NoteCoordinate snappedCoordinate) {
    auto noteSnapshot = createNoteSnapShot(snappedCoordinate);
    stretchOperation.initStretchingNotes(noteSnapshot.absoluteTime,noteSnapshot.endAbsoluteTime);
    stretchOperation.addNotes(noteSnapshot);

}

void PianoRollComponent::renderPattern(const TimelineContext& ctx) const{
    auto* pattern = &PatternManager::instance().getCurrentPattern();

    //pattern->
    for (auto noteIDs : pattern->m_noteEvents) {
        auto onEvent = pattern->getMidiEventByID_ptr(noteIDs.onID);

        if (pattern->m_hiddenNoteIDs.contains(noteIDs.onID))
        {
            continue;
        }
        auto offEvent = pattern->getMidiEventByID_ptr(noteIDs.offID);

        float startDelta =0;
        if (onEvent->m_absoluteTime != 0){startDelta =
            static_cast<float>(onEvent->m_absoluteTime) / TimeData::PPQ;}

        float endDelta=0;
        if (offEvent->m_absoluteTime != 0){endDelta =
            static_cast<float>(offEvent->m_absoluteTime) / TimeData::PPQ;}

        float startPixel = view_state->getPixelPerBeat(zoomFactor::pianoRoll) * startDelta;
        float endPixel = view_state->getPixelPerBeat(zoomFactor::pianoRoll) * endDelta ;

        float yStart = ctx.cursorPos.y + (ctx.height - (onEvent->getPitch()+1)*ctx.noteHeight);
        float xStart = ctx.cursorPos.x + startPixel;
        float xEnd =  ctx.cursorPos.x + endPixel;
        float yEnd = yStart + ctx.noteHeight;

        auto colour = Theme::currentThemeColours.barColourPacked;
        for (auto id : pattern->m_selectedNoteIDs) {
            if (noteIDs.onID == id) {
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

void PianoRollComponent::renderPlaceHolderNotes(const TimelineContext& ctx){
    std::vector<NoteSnapshot> PlaceHolderNotes;

    if (moveOperation.isMovingNote) {
        PlaceHolderNotes.insert(PlaceHolderNotes.end() ,moveOperation.movingNotes.begin(),moveOperation.movingNotes.end());
    }
    else if (stretchOperation.isStretchingNote) {
        PlaceHolderNotes.insert(PlaceHolderNotes.end(),stretchOperation.stretchingNotes.begin(),stretchOperation.stretchingNotes.end());
    }
    else{return;}

    for (auto i{0u}; i < PlaceHolderNotes.size(); ++i) {
        auto note = PlaceHolderNotes.at(i);

        auto onTime = note.absoluteTime;
        auto offTime = note.endAbsoluteTime;
        auto pitch = note.pitch;
        if ( i < moveOperation.movingNotes.size() ) {
            onTime +=  moveOperation.newDeltaTime;
            offTime +=  moveOperation.newDeltaTime;
            pitch +=  moveOperation.newDeltaPitch;
        }
        else if ( i < stretchOperation.stretchingNotes.size() ) {
            offTime += stretchOperation.newEndDelta;
        }

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