#include <algorithm>
#include "PianoRollComponent.h"
#include "../../../NoteOperations.h"
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

NoteCoordinate PianoRollComponent::resolveHoverCoordinate(const TimelineContext& ctx) const {
    int absoluteTime = static_cast<int>(ctx.relativeX / view_state->getPixelPerBeat(pianoRoll) * TimeData::PPQ);
    int pitch = std::clamp(127 - static_cast<int>(ctx.relativeY / ctx.noteHeight), 0, 127);
    return NoteCoordinate(pitch, static_cast<uint32_t>(absoluteTime));
}

NoteCoordinate PianoRollComponent::resolveSnappedCoordinate(const TimelineContext& ctx) const {
    int noteTime = static_cast<int>(ctx.relativeX / view_state->getPixelPerBeat(pianoRoll)
                                    * view_state->getSnappedSubDivisions());
    int snappedTime = noteTime * (static_cast<float>(TimeData::PPQ) / view_state->getSnappedSubDivisions());
    int pitch = std::clamp(127 - static_cast<int>(ctx.relativeY / ctx.noteHeight), 0, 127);
    return NoteCoordinate(pitch, static_cast<uint32_t>(snappedTime));
}

void PianoRollComponent::edit(const TimelineContext& ctx)
{
    const NoteCoordinate hover = resolveHoverCoordinate(ctx);
    const NoteCoordinate snapped = resolveSnappedCoordinate(ctx);
    const auto hoverState = PatternManager::instance().getNoteHoverState(hover);


    if (moveOperation.isActive) { updateMoveOperation(snapped);}
    else if (stretchOperation.isActive) { updateStretchOperation(snapped);}
    else if (scaleOperation.isActive)
    {
        updateScaleOperation(snapped);
    }


    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        if (isScaleHandleHover(hover)){ scaleNote(hover);}
        switch (hoverState) {
        case NoteCenterHover: moveNote(snapped);    break;
        case NoteEdgeHover:   stretchNote(snapped); break;
        default:              sendNewNote(snapped); break;
        }
    }
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        PatternManager::instance().removeNoteFromPattern(hover);
    }
}

bool PianoRollComponent::isScaleHandleHover(NoteCoordinate hover){
    auto p1 = ToolManager::instance().getScaleHandlePosition();
    auto p2 = ToolManager::instance().getScaleHandleSize();
    auto s1 = ImVec2((static_cast<float>(hover.absoluteTime) /TimeData::PPQ) * view_state->getPixelPerBeat(pianoRoll),(127 - hover.pitch)*view_state->getNoteHeight());

    if ( s1.x > p1.x && s1.x < p2.x) {
        if ( s1.y > p1.x && s1.y < p2.x) {
            std::cout<<"clicked on "<<std::endl;
            scaleOperation.addNotes(setupSnapshots(hover));
            return true;
        }
    }
    return false;
}

void PianoRollComponent::updateMoveOperation(NoteCoordinate snapped){
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left))        {
        moveOperation.update(snapped);
    }
    else{
        auto commit = moveOperation.commit();
        if (std::holds_alternative<SingleNoteCommit>(commit))  {
            auto& c = std::get<SingleNoteCommit>(commit);
            PatternManager::instance().moveNoteEvent(c.ID, c.coord);
        } else {
            auto& c = std::get<NotesCommit>(commit);
            PatternManager::instance().moveSelection(c.delta);
        }
        PatternManager::instance().showAllEvents();
    }
}
void PianoRollComponent::updateStretchOperation(NoteCoordinate snapped){
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left)){
        stretchOperation.update(snapped);
    }
    else{
        auto commit = stretchOperation.commit();
        if (std::holds_alternative<SingleNoteCommit>(commit))  {
            auto& c = std::get<SingleNoteCommit>(commit);
            PatternManager::instance().stretchNoteEvent(c.ID, c.coord.absoluteTime);
        } else {
            auto& c = std::get<NotesCommit>(commit);
            PatternManager::instance().stretchSelection(c.delta);
        }
        PatternManager::instance().showAllEvents();
    }
}
void PianoRollComponent::updateScaleOperation(NoteCoordinate snapped)
{
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left)){
        scaleOperation.update(snapped);
    }
    // else{
    //     auto commit = stretchOperation.commit();
    //     if (std::holds_alternative<SingleNoteCommit>(commit))  {
    //         auto& c = std::get<SingleNoteCommit>(commit);
    //         PatternManager::instance().stretchNoteEvent(c.ID, c.coord.absoluteTime);
    //     } else {
    //         auto& c = std::get<NotesCommit>(commit);
    //         PatternManager::instance().stretchSelection(c.delta);
    //     }
    //     PatternManager::instance().showAllEvents();
    //}
}

void PianoRollComponent::sendNewNote(NoteCoordinate snappedCoordinate) {
    PatternManager::instance().addNoteToPattern(snappedCoordinate.pitch,
        static_cast<signed>(snappedCoordinate.absoluteTime),  ToolManager::instance().getLastNoteDuration());
}

void PianoRollComponent::moveNote(NoteCoordinate snappedCoordinate){
    auto noteSnapshots = setupSnapshots(snappedCoordinate);
    moveOperation.originalInputCoordinate = snappedCoordinate;
    moveOperation.addNotes(noteSnapshots);
}

void PianoRollComponent::stretchNote(NoteCoordinate snappedCoordinate) {
    auto noteSnapshots = setupSnapshots(snappedCoordinate);
    stretchOperation.initStretchingNotes(noteSnapshots[0].absoluteTime,noteSnapshots[0].endAbsoluteTime);
    stretchOperation.addNotes(noteSnapshots);
}

void PianoRollComponent::scaleNote(NoteCoordinate snappedCoordinate) {
    auto noteSnapshots = setupSnapshots(snappedCoordinate);
    //scaleOperation.initStretchingNotes(noteSnapshots[0].absoluteTime,noteSnapshots[0].endAbsoluteTime);
    scaleOperation.addNotes(noteSnapshots);
}


std::vector<NoteSnapshot> PianoRollComponent::setupSnapshots(NoteCoordinate snappedCoordinate)
{
    auto* pattern = &PatternManager::instance().getCurrentPattern();
    std::vector<NoteSnapshot> noteSnapshots;
    if (pattern->m_selectedNoteIDs.size() > 0) {
        for (uint32_t m_selected_note_i_d : pattern->m_selectedNoteIDs) {
            PatternManager::instance().hideNoteEventByID(m_selected_note_i_d);
            noteSnapshots.push_back(createNoteSnapShotBasedOnID(m_selected_note_i_d));
        }
    }
    else {
        noteSnapshots.push_back(createNoteSnapShot(snappedCoordinate));
    }
    return noteSnapshots;
}

NoteSnapshot PianoRollComponent::createNoteSnapShotBasedOnID(uint32_t onID) {
    auto* pattern = &PatternManager::instance().getCurrentPattern();
    auto noteIds = *pattern->getEventIDPairFromOnID(onID);
    auto onNote = pattern->getMidiEventByID_ptr(noteIds.onID);
    auto offNote = pattern->getMidiEventByID_ptr(noteIds.offID);

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

    if (moveOperation.isActive) {
        PlaceHolderNotes.insert(PlaceHolderNotes.end() ,moveOperation.notes.begin(),moveOperation.notes.end());
    }
    else if (stretchOperation.isActive) {
        PlaceHolderNotes.insert(PlaceHolderNotes.end(),stretchOperation.notes.begin(),stretchOperation.notes.end());
    }
    else if (scaleOperation.isActive) {
        PlaceHolderNotes.insert(PlaceHolderNotes.end(),scaleOperation.notes.begin(),scaleOperation.notes.end());
    }
    else{return;}

    for (auto i{0u}; i < PlaceHolderNotes.size(); ++i) {
        auto note = PlaceHolderNotes.at(i);

        auto onTime = note.absoluteTime;
        auto offTime = note.endAbsoluteTime;
        auto pitch = note.pitch;
        if ( i < moveOperation.notes.size() ) {
            onTime +=  moveOperation.newDeltaTime;
            offTime +=  moveOperation.newDeltaTime;
            pitch +=  moveOperation.newDeltaPitch;
        }
        else if ( i < stretchOperation.notes.size()) {
            if (offTime + stretchOperation.newEndDelta < onTime) {
                offTime = onTime + ViewState::instance().getStandardSnapTime();
            }
            else{offTime += stretchOperation.newEndDelta;}
        }
        else if (i < scaleOperation.notes.size()) {
            onTime = onTime * scaleOperation.scale;
            offTime = offTime * scaleOperation.scale;
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
    if (PatternManager::instance().areNotesSelected()){
        auto tool = &ToolManager::instance();
        const auto pattern = &PatternManager::instance().getCurrentPattern();
        auto middlePointY = 0;
        uint32_t endPointX = 0;
        for (auto onID :pattern->m_selectedNoteIDs)
        {
            middlePointY += 127 - pattern->getMidiEventByID_ptr(onID)->getPitch();
            auto note = pattern->getMidiEventByID_ptr(pattern->getEventIDPairFromOnID(onID)->offID);
            if (note->getAbsoluteTime() > endPointX){
                endPointX = note->getAbsoluteTime();
            }
        }
        middlePointY = (middlePointY*ctx.noteHeight) / pattern->m_selectedNoteIDs.size();
        endPointX = ((endPointX+ TimeData::PPQ)/TimeData::PPQ) * view_state->getPixelPerBeat(zoomFactor::pianoRoll) ;
        tool->setScaleHandlePosition(ImVec2(endPointX, middlePointY));

        ImVec2 endCoords(tool->getScaleHandlePosition().x + tool->getScaleHandleSize().x + ctx.cursorPos.x,
            tool->getScaleHandlePosition().y + tool->getScaleHandleSize().y + ctx.cursorPos.y);

        ctx.drawList->AddRectFilled(
        ImVec2(tool->getScaleHandlePosition().x + ctx.cursorPos.x,
            tool->getScaleHandlePosition().y + ctx.cursorPos.y),
            endCoords,
        Theme::currentThemeColours.barColourPacked, 0.0f);
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
        else if (ImGui::IsKeyPressed(ImGuiKey_A))
        {

        }
    }
    if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {
        if (PatternManager::instance().areNotesSelected()){
            PatternManager::instance().deleteSelection();
        }
        else{
            PatternManager::instance().clearPattern();
        }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_E)){
        ToolManager::instance().setPianoRollTool(ToolTypes::EDIT);
    }
    else if (ImGui::IsKeyPressed(ImGuiKey_S))    {
        ToolManager::instance().setPianoRollTool(ToolTypes::SELECT);
    }

}