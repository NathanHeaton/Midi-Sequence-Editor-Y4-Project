#include <algorithm>
#include "PianoRollComponent.h"
#include "../../../NoteOperations.h"
#include "../../../utils.h"
//
// Created by nathan on 16/02/2026.
//

void PianoRollComponent::renderPattern(const TimelineContext& ctx) const{
    auto* pattern = PatternManager::instance().getPatternByID(m_patternID);

    //pattern->
    for (auto noteIDs : pattern->m_noteEvents) {
        auto onEvent = pattern->getMidiEventByID_ptr(noteIDs.onID);
        if (pattern->m_hiddenNoteOnIDs.contains(noteIDs.onID)) continue;
        auto offEvent = pattern->getMidiEventByID_ptr(noteIDs.offID);

        float startDelta =0;
        if (onEvent->m_absoluteTime != 0){startDelta =
            static_cast<float>(onEvent->m_absoluteTime) / TimeData::PPQ;}

        float endDelta=0;
        if (offEvent->m_absoluteTime != 0){endDelta =
            static_cast<float>(offEvent->m_absoluteTime) / TimeData::PPQ;}

        float startPixel = view_state->getPixelPerBeat(pianoRoll) * startDelta;
        float endPixel = view_state->getPixelPerBeat(pianoRoll) * endDelta ;

        if (endPixel < ctx.scrollX){ continue;}
        if (startPixel > ctx.width + ctx.scrollX + ctx.width/3){break;}

        float yStart = ctx.cursorPos.y + (ctx.height - (onEvent->getPitch()+1)*ctx.noteHeight);
        float xStart = ctx.cursorPos.x + startPixel;
        float xEnd =  ctx.cursorPos.x + endPixel;
        float yEnd = yStart + ctx.noteHeight;

        auto colour = pattern->m_selectedNoteOnIDs.contains(noteIDs.onID)
        ? Theme::cTheme.beatColourPacked
        : Theme::cTheme.barColourPacked;

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
            onTime +=  static_cast<uint32_t>(moveOperation.newDeltaTime);
            offTime +=  static_cast<uint32_t>(moveOperation.newDeltaTime);
            pitch +=  moveOperation.newDeltaPitch;
        }
        else if ( i < stretchOperation.notes.size()) {
            if (offTime + static_cast<uint32_t>(stretchOperation.newEndDelta) < onTime) {
                offTime = onTime + ViewState::instance().getStandardSnapTime();
            }
            else{offTime += static_cast<uint32_t>(stretchOperation.newEndDelta);}
        }
        else if (i < scaleOperation.notes.size()) {
            auto firstScaledNote = scaleOperation.firstNoteAbsolute;
            onTime = firstScaledNote + static_cast<uint32_t>((onTime - firstScaledNote) *
                scaleOperation.scale);
            offTime = firstScaledNote + static_cast<uint32_t>((offTime - firstScaledNote ) * scaleOperation.scale);
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
        Theme::cTheme.beatColourPacked, 3.0f);
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
                Theme::cTheme.selectionPacked, 0.0f);
        }
    }
    if (PatternManager::instance().areNotesSelected(m_patternID)){ // render scale effect
        auto tool = &ToolManager::instance();
        const auto pattern = PatternManager::instance().getPatternByID(m_patternID);
        auto middlePointY = 0;
        uint32_t endPointX = 0;
        for (auto onID :pattern->m_selectedNoteOnIDs)
        {
            middlePointY += 127 - pattern->getMidiEventByID_ptr(onID)->getPitch();
            auto note = pattern->getMidiEventByID_ptr(pattern->getEventIDPairFromOnID(onID)->offID);
            if (note->getAbsoluteTime() > endPointX){
                endPointX = note->getAbsoluteTime();
            }
        }
        middlePointY = (middlePointY*ctx.noteHeight) / pattern->m_selectedNoteOnIDs.size();
        endPointX = ((endPointX+ TimeData::PPQ)/TimeData::PPQ) * view_state->getPixelPerBeat(zoomFactor::pianoRoll);

        tool->setScaleHandlePosition(ImVec2(endPointX+ ctx.cursorPos.x, middlePointY+ ctx.cursorPos.y));
        ImVec2 endCoords = addImVec2(tool->getScaleHandlePosition(), tool->getScaleHandleSize());

        ctx.drawList->AddRectFilled(
        tool->getScaleHandlePosition(),
            endCoords,
        Theme::cTheme.barColourPacked, 0.0f);
    }
}
