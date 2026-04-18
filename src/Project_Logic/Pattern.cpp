#include "Pattern.h"

#include <algorithm>
#include "../Singletons/TimeData.h"
#include "../Singletons/ViewState.h"
#include <cmath>
#include <ranges>
#include <sys/stat.h>

#include "../Singletons/ToolManager.h"
#include "../Singletons/PatternManager.h"

//
// Created by nathan on 11/01/2026.
//

void Pattern::setLastBar() {
    auto barSize = TimeData::instance().PPQ * TimeData::instance().timeSignature.getNumerator();
    int endAbsolute = m_events.back().m_absoluteTime;

    float bars = static_cast<float>(endAbsolute) / static_cast<float>(barSize);
    std::cout << "before setting bars" << std::endl;
    m_bars = ceil(bars);
    std::cout << "after setting bars" << std::endl;
}

void Pattern::createNoteEventPairs(){
    if (m_events.empty()) {
        return;
    }
    struct PendingNoteEvent {
        uint8_t pitch;
        uint8_t channel;
        size_t onID;
        PendingNoteEvent(size_t t_onID,uint8_t t_pitch, uint8_t t_chan) {
            onID = t_onID;
            pitch = t_pitch;
            channel = t_chan;
        }

    };
    std::vector<PendingNoteEvent> pendingEvents;
    int cumulativeTime=0;
    for (auto& event : m_events) {
        cumulativeTime += event.getDelta();
        event.m_absoluteTime = cumulativeTime;
        if (event.isNoteOff()||
            (event.getVelocity() == 0 && event.isNoteOn())) {
            for (auto it{pendingEvents.begin()};it< pendingEvents.end();it++) {
                if (event.getPitch() == it->pitch &&
                    event.getChannel() == it->channel) {
                    m_noteEvents.emplace_back(it->onID, event.getID());
                    pendingEvents.erase(it);
                    break;}
            }
        }
        else if (event.isNoteOn()) {
            pendingEvents.emplace_back(event.getID(),event.getPitch(),event.getChannel());
        }
    }
    rebuildNoteIndices();

    m_pitchRange = {};
    for (const auto& pair : m_noteEvents) {
        uint8_t pitch = getMidiEventByID_ptr(pair.onID)->getPitch();
        m_pitchRange.lowest  = std::min(m_pitchRange.lowest,  pitch);
        m_pitchRange.highest = std::max(m_pitchRange.highest, pitch);
    }
}

void Pattern::convertMidiTicksToPPQ() {
    for (auto& event : m_events) {
        uint32_t newDelta = static_cast<uint32_t>(event.getDelta()/(static_cast<float>(ticksInMidiFile))*
            TimeData::instance().PPQ);
        event.setDelta(newDelta);
    }
}

std::optional<NoteEventPair> Pattern::findNoteBasedOnPoint(NoteCoordinate noteCoordinate) {
    if (m_events.empty()) {
        return std::nullopt;
    }
    for (const auto noteP : m_noteEvents){
        auto onNote = getMidiEventByID_ptr(noteP.onID);
        auto offNote = getMidiEventByID_ptr(noteP.offID);
        if (onNote->getAbsoluteTime() <= noteCoordinate.absoluteTime &&
            offNote->getAbsoluteTime() >= noteCoordinate.absoluteTime) {
            if (onNote->getPitch() == noteCoordinate.pitch) {
                return noteP;
            }
        }
    }
    return std::nullopt;
}

void Pattern::removeNote(NoteEventPair note) {
    if (m_events.empty()) {
        return;
    }
    removeNoteOperation(note);
    m_selectedNoteOnIDs.clear();
    m_hiddenNoteOnIDs.clear();
}

void Pattern::removeSelection(std::vector<NoteCoordinate> events ) {
    for (auto& event:events) {
        auto notePair = findNoteBasedOnPoint(event);
        if (getEventIndexByID(notePair->offID) == SIZE_MAX) {
            return;
        }
        removeNote(*notePair);
    }
    m_selectedNoteOnIDs.clear();
    m_hiddenNoteOnIDs.clear();
    m_noteEvents.clear();
    createNoteEventPairs();
}

void Pattern::deleteSelection() {
    if (m_selectedNoteOnIDs.empty()) {
        return;
    }
    for ( auto noteIds : m_noteEvents) {
        if (m_selectedNoteOnIDs.contains(noteIds.onID)) {
            removeNoteOperation(noteIds);
        }
    }
    m_selectedNoteOnIDs.clear();
    m_noteEvents.clear();
    createNoteEventPairs();
}

void Pattern::timeShiftSelection(int32_t t_timeDelta) {
    if (m_selectedNoteOnIDs.empty()) {
        return;
    }
    timeShiftOperation(t_timeDelta,m_selectedNoteOnIDs);

}

void Pattern::timeShiftOperation(uint32_t t_timeDelta, std::unordered_set<uint32_t> IDs) {
    std::vector<MidiEvent> tempEvents;
    for ( auto noteIds : m_noteEvents) {
        if (m_selectedNoteOnIDs.contains(noteIds.onID)) {
            auto onNote = *getMidiEventByID_ptr(noteIds.onID);
            auto offNote = *getMidiEventByID_ptr(noteIds.offID);
            tempEvents.push_back(onNote);
            tempEvents.push_back(offNote);
            removeNoteOperation(noteIds);
        }
    }

    for ( auto event : tempEvents) {
        insertEvent(event,event.getAbsoluteTime() + t_timeDelta);
    }
    m_noteEvents.clear();
    createNoteEventPairs();
}



void Pattern::calculateSelection(const SelectionCoords &t_selection) {
    m_selectedNoteOnIDs.clear();

    float pixelPerTick = TimeData::instance().PPQ / ViewState::instance().getPixelPerBeat(zoomFactor::pianoRoll);
    float noteHeight = ViewState::instance().getNoteHeight();

    ImVec2 adjustedValuesP1 = {t_selection.selectP1.x * pixelPerTick,
        t_selection.selectP1.y / noteHeight};
    ImVec2 adjustedValuesP2 = {t_selection.selectP2.x * pixelPerTick,
        t_selection.selectP2.y / noteHeight};

    for (auto& note : m_noteEvents) {
        auto onX = static_cast<float>(getMidiEventByID_ptr(note.onID)->getAbsoluteTime());
        auto offX = static_cast<float>(getMidiEventByID_ptr(note.offID)->getAbsoluteTime());
        auto onY = 127 - static_cast<float>(getMidiEventByID_ptr(note.onID)->getPitch());

        if ((onX > adjustedValuesP1.x || offX > adjustedValuesP1.x) && onX < adjustedValuesP2.x) {
            if (onY > adjustedValuesP1.y && onY+1  < adjustedValuesP2.y) {
                m_selectedNoteOnIDs.emplace(note.onID);
            }
        }
    }

}

// Finds the insertion index for an event at the given absolute time,
// and adjusts the delta of the following event to maintain correct spacing.
// Returns the index at which the new event should be inserted.
size_t Pattern::findInsertionPoint(uint32_t absoluteTime) {
    size_t index = 0;

    for (index = 0; index < m_events.size(); index++) {
        if (m_events[index].getAbsoluteTime() >= absoluteTime) {
            break;
        }
    }

    if (index < m_events.size()) {
        uint32_t followingDelta = m_events[index].getAbsoluteTime() - absoluteTime;
        m_events[index].setDelta(followingDelta);
    }

    return index;
}

// Calculates the delta for a new event being inserted at the given index and absolute time.
// Delta is relative to the preceding event, or absolute if there is no preceding event.

uint32_t Pattern::calculateDelta(size_t insertionIndex, uint32_t absoluteTime) const {
    if (insertionIndex == 0 || m_events.empty()) {
        return absoluteTime;
    }

    return absoluteTime - m_events[insertionIndex - 1].getAbsoluteTime();
}

// Inserts a MidiEvent at the correct position in m_events based on its absolute time,
// adjusting surrounding deltas to keep the sequence consistent.
void Pattern::insertEvent(MidiEvent& event, uint32_t absoluteTime) {
    size_t index    = findInsertionPoint(absoluteTime);
    uint32_t delta  = calculateDelta(index, absoluteTime);
    event.setDelta(delta);
    event.m_absoluteTime = absoluteTime;
    m_events.insert(m_events.begin() + index, event);
    updateBarCount(absoluteTime);
}

void Pattern::setNoteVelocity(NoteEventPair notePair, uint8_t velocity) {
    getMidiEventByID_ptr(notePair.onID)->setVelocity(velocity);
    getMidiEventByID_ptr(notePair.offID)->setVelocity(velocity);
}

void Pattern::removeNoteOperation(NoteEventPair notepair) {
    auto onNote = getMidiEventByID_ptr(notepair.onID);
    auto offNote = getMidiEventByID_ptr(notepair.offID);
    auto onDelta = onNote->getDelta();

    auto noteOnIndex = getEventIndexByID(notepair.onID);
    auto noteOffIndex = getEventIndexByID(notepair.offID);

    if (noteOnIndex+1 < m_events.size()) {
        m_events[noteOnIndex+1].setDelta(m_events[noteOnIndex+1].getDelta() +onDelta );
    }
    auto offDelta = offNote->getDelta();
    if (noteOffIndex +1< m_events.size()) {
        m_events[noteOffIndex+1].setDelta(m_events[noteOffIndex+1].getDelta() +offDelta );
    }

    size_t first  = std::min(noteOnIndex, noteOffIndex);
    size_t second = std::max(noteOnIndex, noteOffIndex);

    m_events.erase(m_events.begin() + second);
    m_events.erase(m_events.begin() + first);

}

void Pattern::addNote(uint8_t t_pitch, uint32_t absoluteTime, uint32_t duration) {
    const uint8_t channel  = 0;
    const uint8_t velocity = 127;
    const uint32_t offAbsoluteTime = absoluteTime + duration;

    MidiEvent noteOn(0, 0x90, Note{t_pitch, velocity}, channel);
    noteOn.setID(assignID());
    insertEvent(noteOn, absoluteTime);

    MidiEvent noteOff(0, 0x80, Note{t_pitch, velocity}, channel);
    noteOff.setID(assignID());
    insertEvent(noteOff, offAbsoluteTime);

    m_noteEvents.clear();
    createNoteEventPairs();
}


void Pattern::pitchShiftSelection(signed short t_pitchDelta) {
    for (const auto selectionID: m_selectedNoteOnIDs) {
        for (const auto& pair: m_noteEvents) {
            auto onEvent = getMidiEventByID_ptr(pair.onID);
            auto newPitch = std::clamp(onEvent->getPitch() + t_pitchDelta,0,127);
            if (selectionID == pair.onID) {
                onEvent->setPitch(newPitch);
                getMidiEventByID_ptr(pair.offID)->setPitch(newPitch);
            }
        }
    }
}


HoverState Pattern::findNoteHoverState(NoteCoordinate hoverCoordinate) {
    HoverState state = NoHover;
    for (const auto& pair: m_noteEvents) {
        const auto onEvent = getMidiEventByID_ptr(pair.onID);
        const auto offEvent = getMidiEventByID_ptr(pair.offID);
        if (onEvent->getPitch() == hoverCoordinate.pitch) {
            if (onEvent->getAbsoluteTime() <= hoverCoordinate.absoluteTime&&
                offEvent->getAbsoluteTime() >= hoverCoordinate.absoluteTime ) {
                auto centerRegion = offEvent->getAbsoluteTime() -
                    (offEvent->getAbsoluteTime() - onEvent->getAbsoluteTime()) * 0.2;
                auto hoverDelta = hoverCoordinate.absoluteTime;
                if (hoverDelta <= centerRegion ) {
                    state = CenterHover;
                }
                else {
                    state = EdgeHover;
                }
                break;
            }
        }
    }

    return state;
}

void Pattern::pasteClipboard(std::vector<MidiEvent>& copied_events, NoteMoveDelta offSet){
    m_selectedNoteOnIDs.clear();
    for (auto event: copied_events){
        event.setID(assignID());
        if (event.isNoteOn()){m_selectedNoteOnIDs.emplace(event.getID());}
        event.setPitch(event.getPitch() + offSet.pitchD);
        event.setAbsoluteTime(event.getAbsoluteTime() + offSet.timeDelta);
        insertEvent(event, event.getAbsoluteTime());
    }
    m_noteEvents.clear();
    createNoteEventPairs();
}

void Pattern::hideNoteEvent(NoteCoordinate coordinate)
{
    for (const auto& pair: m_noteEvents)    {
        const auto onEvent = getMidiEventByID_ptr(pair.onID);
        const auto offEvent = getMidiEventByID_ptr(pair.offID);
        if (onEvent->getPitch() == coordinate.pitch)
        {
            if (onEvent->getAbsoluteTime() <= coordinate.absoluteTime&&
                offEvent->getAbsoluteTime() >= coordinate.absoluteTime ) {
                    m_hiddenNoteOnIDs.insert(pair.onID);
                }
        }
    }
}

void Pattern::fixDeltaFromDeletedNote(NoteEventPair* pair)
{
    // auto onIndex = pair->onID;
    // auto offIndex = pair->offID;
    //
    // if (onIndex < m_events.size()) {
    //     if (m_events[onIndex].getAbsoluteTime() < m_events[onIndex+1].getAbsoluteTime())
    //     {
    //         uint32_t followingDelta = m_events[onIndex+1].getAbsoluteTime() - m_events[onIndex].getAbsoluteTime() ;
    //         m_events[onIndex+1].setDelta(followingDelta);
    //     }
    // }
    // if (offIndex < m_events.size()) {
    //     if (m_events[offIndex].getAbsoluteTime() < m_events[offIndex+1].getAbsoluteTime())
    //     {
    //         uint32_t followingDelta = m_events[offIndex+1].getAbsoluteTime() - m_events[offIndex].getAbsoluteTime() ;
    //         m_events[offIndex+1].setDelta(followingDelta);
    //     }
    // }
}

void Pattern::moveNoteEvent(uint32_t ID, NoteCoordinate pos)
{
    auto notePair = findPairByID(ID);

    MidiEvent movedOnEvent = *getMidiEventByID_ptr(notePair.onID);
    MidiEvent movedOffEvent = *getMidiEventByID_ptr(notePair.offID);
    movedOnEvent.setPitch(pos.pitch);
    movedOffEvent.setPitch(pos.pitch);
    removeNote(notePair);
    if ( pos.absoluteTime < 0) {
        pos.absoluteTime = 0;
    }
    m_noteEvents.clear();
    createNoteEventPairs();

    uint32_t endAbsolute = movedOffEvent.getAbsoluteTime() + (pos.absoluteTime - movedOnEvent.getAbsoluteTime());

    insertEvent(movedOnEvent, static_cast<unsigned>(pos.absoluteTime));
    insertEvent(movedOffEvent, static_cast<unsigned>(endAbsolute));

    m_noteEvents.clear();
    createNoteEventPairs();

}
void Pattern::updateBarCount(uint32_t endAbsolute) {
    if (endAbsolute >= (barSizeTicks * m_bars)) {
        setLastBar();
    }
}
void Pattern::moveNoteEventSelection(NoteMoveDelta coordinateDelta) {
    timeShiftOperation(coordinateDelta.timeDelta,m_selectedNoteOnIDs);
    pitchShiftSelection(coordinateDelta.pitchD);
    m_selectedNoteOnIDs.clear();
}

void Pattern::stretchNoteEventSelection(NoteMoveDelta coordinateDelta)
{
    for (uint32_t id : m_selectedNoteOnIDs)
    {
        stretchNoteEvent(id,coordinateDelta.timeDelta);
    }
}

void Pattern::stretchNoteEvent(uint32_t ID, int32_t newEndDelta) {
    auto notePair = findPairByID(ID);
    MidiEvent movedOnEvent = *getMidiEventByID_ptr(notePair.onID);
    MidiEvent movedOffEvent = *getMidiEventByID_ptr(notePair.offID);

    removeNoteOperation(notePair);

    insertEvent(movedOnEvent, movedOnEvent.getAbsoluteTime());
    if (movedOnEvent.getAbsoluteTime() >= movedOffEvent.getAbsoluteTime() + newEndDelta) {
        std::cout << "Had to fix note end time" << ID << std::endl;
        insertEvent(movedOffEvent, movedOnEvent.getAbsoluteTime() + ViewState::instance().getStandardSnapTime());
    }
    else{insertEvent(movedOffEvent, movedOffEvent.getAbsoluteTime() + newEndDelta);}
    m_noteEvents.clear();
    createNoteEventPairs();
}

void Pattern::scaleNoteEventSelection(float scale) {
    if (m_selectedNoteOnIDs.empty() || scale <= 0.0f)
        return;

    std::vector<MidiEvent> tempEvents;

    for (const auto& pair : m_noteEvents) {
        if (!m_selectedNoteOnIDs.contains(pair.onID))
            continue;
        tempEvents.push_back(*getMidiEventByID_ptr(pair.onID));
        tempEvents.push_back(*getMidiEventByID_ptr(pair.offID));
        removeNoteOperation(pair);
    }

    for (auto& event : tempEvents) {
        auto newTime = static_cast<uint32_t>(event.getAbsoluteTime() * scale);
        insertEvent(event, newTime);
    }
    m_selectedNoteOnIDs.clear();
    m_noteEvents.clear();
    createNoteEventPairs();
}