#include "Pattern.h"

#include <algorithm>

#include "../Singletons/TimeData.h"
#include "../Singletons/ViewState.h"
#include <iostream>
#include <cmath>
#include <ranges>
#include <sys/stat.h>

#include "../Singletons/ToolManager.h"
#include "../Singletons/PatternManager.h"
//
// Created by nathan on 11/01/2026.
//

void Pattern::setLastBar() {
    const auto barSize = TimeData::instance().PPQ * TimeData::instance().timeSignature.getNumerator();
    int endAbsolute = m_events.back().m_absoluteTime;

    float bars = static_cast<float>(endAbsolute) / static_cast<float>(barSize);
    m_bars = ceil(bars);
}

void Pattern::createNoteEventPairs(){
    if (m_events.empty()) {
        return;
    }
    struct PendingNoteEvent {
        uint8_t pitch;
        uint8_t channel;
        size_t onIndex;
        PendingNoteEvent(size_t t_onIndex,uint8_t t_pitch, uint8_t t_chan) {
            onIndex = t_onIndex;
            pitch = t_pitch;
            channel = t_chan;
        }
    };
    std::vector<PendingNoteEvent> pendingEvents;
    int cumulativeTime=0;

    for (size_t i=0; i < m_events.size(); i++) {
        cumulativeTime += m_events.at(i).getDelta();
        m_events.at(i).m_absoluteTime = cumulativeTime;
        if (m_events.at(i).isNoteOff() || (m_events.at(i).getVelocity() == 0 && m_events.at(i).isNoteOn())) {
            for (auto it{pendingEvents.begin()};it< pendingEvents.end();it++) {
                if (m_events.at(i).getPitch() == it->pitch &&
                    m_events.at(i).getChannel() == it->channel) {
                    m_noteEvents.emplace_back(it->onIndex, i);
                    pendingEvents.erase(it);
                    break;}
            }
        }
        else if (m_events.at(i).isNoteOn()) {
            pendingEvents.emplace_back(i,m_events.at(i).getPitch(),m_events.at(i).getChannel());
        }
    }
}

void Pattern::convertMidiTicksToPPQ() {
    for (auto& event : m_events) {
        uint32_t newDelta = static_cast<uint32_t>(event.getDelta()/(static_cast<float>(ticksInMidiFile))*
            TimeData::instance().PPQ);
        event.setDelta(newDelta);
    }
}

NoteEventPair Pattern::findNoteBasedOnPoint(uint8_t t_pitch, uint32_t absoluteTime) {
    if (m_events.empty()) {
        return NoteEventPair(0,0);
    }
    for (auto i{0u};i<m_noteEvents.size();i++){
        auto& note = m_noteEvents.at(i);
        auto& onNote = m_events.at(note.onIndex);
        auto& offNote = m_events.at(note.offIndex);
        if (onNote.m_absoluteTime <= absoluteTime &&
            offNote.m_absoluteTime >= absoluteTime) {
            if (onNote.getPitch() == t_pitch) {
                return note;
            }
        }
    }
    return NoteEventPair(0,0);

}

void Pattern::removeNote(NoteEventPair notePair) {
    if (m_events.empty()) {
        return;
    }
     auto& onNote = m_events.at(notePair.onIndex);
     auto& offNote = m_events.at(notePair.offIndex);
     auto onDelta = onNote.getDelta();

     if (notePair.onIndex + 1 < m_events.size()) {

         m_events[notePair.onIndex + 1].setDelta(m_events[notePair.onIndex + 1].getDelta() +onDelta );
     }

     auto offDelta = offNote.getDelta();
     if (notePair.offIndex + 1 < m_events.size()) {
         m_events[notePair.offIndex + 1].setDelta(m_events[notePair.offIndex + 1].getDelta() +offDelta );
     }

     m_events.erase(m_events.begin() + static_cast<int>(notePair.offIndex));
     m_events.erase(m_events.begin() + static_cast<int>(notePair.onIndex));

    m_noteEvents.clear();
    createNoteEventPairs();
}

//void Pattern::adjust

void Pattern::removeSelection(noteCoordinate event) {
    auto notePair = findNoteBasedOnPoint(event.pitch,event.absoluteTime);
    if (notePair.offIndex == 0 && notePair.onIndex == 0) {
        return;
    }
    removeNote(notePair);
    m_noteEvents.clear();
    createNoteEventPairs();
}

void Pattern::removeSelection(std::vector<noteCoordinate> events ) {
    for (auto& event:events) {
        auto notePair = findNoteBasedOnPoint(event.pitch,event.absoluteTime);
        if (notePair.offIndex == 0 && notePair.onIndex == 0) {
            return;
        }
        removeNote(notePair);
    }
    m_noteEvents.clear();
    createNoteEventPairs();
}

void Pattern::deleteSelection() {
    if (m_selectedNoteIDs.empty()) {
        return;
    }
    //Todo: find better solution for deleting selection, more robust way of getting selected note ids as they are being updated
    auto selectedNoteIndices = convertNoteIdsToNotePair();
    for (auto i{0u};i<selectedNoteIndices.size();i++) {
        removeNote(selectedNoteIndices.at(i));
        selectedNoteIndices = convertNoteIdsToNotePair();
    }

    m_noteEvents.clear();
    createNoteEventPairs();

}

void Pattern::calculateSelection(const SelectionCoords &t_selection) {
    m_selectedNoteIDs.clear();

    float pixelPerTick = TimeData::instance().PPQ / ViewState::instance().getPixelPerBeat(zoomFactor::pianoRoll);
    float noteHeight = ViewState::instance().getNoteHeight();

    ImVec2 adjustedValuesP1 = {t_selection.selectP1.x * pixelPerTick,
        t_selection.selectP1.y / noteHeight};
    ImVec2 adjustedValuesP2 = {t_selection.selectP2.x * pixelPerTick,
        t_selection.selectP2.y / noteHeight};

    for (auto& pair : m_noteEvents) {
        auto onX = static_cast<float>(m_events.at(pair.onIndex).getAbsoluteTime());
        auto offX = static_cast<float>(m_events.at(pair.offIndex).getAbsoluteTime());
        auto onY = 127 - static_cast<float>(m_events.at(pair.onIndex).getPitch());

        if ((onX > adjustedValuesP1.x || offX > adjustedValuesP1.x) && onX < adjustedValuesP2.x) {
            if (onY > adjustedValuesP1.y && onY+1  < adjustedValuesP2.y) {
                m_selectedNoteIDs.emplace(m_events.at(pair.onIndex).getID());
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
        if (m_events[index].getAbsoluteTime() > absoluteTime) {
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
}

void Pattern::addNote(uint8_t t_pitch, uint32_t absoluteTime, uint32_t duration, uint32_t t_id) {
    const uint8_t channel  = 0;
    const uint8_t velocity = 127;
    const uint32_t offAbsoluteTime = absoluteTime + duration;

    MidiEvent noteOn(0, 0x90, Note{t_pitch, velocity}, channel);
    noteOn.setID(t_id);
    insertEvent(noteOn, absoluteTime);

    MidiEvent noteOff(0, 0x80, Note{t_pitch, velocity}, channel);
    insertEvent(noteOff, offAbsoluteTime);

    m_noteEvents.clear();
    createNoteEventPairs();
}


void Pattern::pitchShiftSelection(signed short t_pitchDelta) {
    for (const auto selectionID: m_selectedNoteIDs) {
        for (const auto& pair: m_noteEvents) {
            auto& onEvent =m_events.at(pair.onIndex);
            auto newPitch = std::clamp(onEvent.getPitch() + t_pitchDelta,0,127);
            if (selectionID == onEvent.getID()) {
                onEvent.setPitch(newPitch);
                m_events.at(pair.offIndex).setPitch(newPitch);
            }
        }
    }
}

void Pattern::timeShiftSelection(int32_t t_timeDelta) {
    //todo: fix note deltas changing after time shift past unselected notes
    for (const auto selectionID: m_selectedNoteIDs) {
        for (const auto& pair: m_noteEvents) {
            auto onEvent = m_events.at(pair.onIndex);
            if (selectionID == onEvent.getID()) {
                auto offEvent = m_events.at(pair.offIndex);

                auto OnAbsoluteTime = static_cast<signed>(onEvent.getAbsoluteTime()) + t_timeDelta;
                auto OffAbsoluteTime = static_cast<signed>(offEvent.getAbsoluteTime()) + t_timeDelta;
                if (OnAbsoluteTime < 0) OnAbsoluteTime = 0u;
                if (OffAbsoluteTime == OnAbsoluteTime+OffAbsoluteTime) OffAbsoluteTime -= t_timeDelta;

                m_events.erase(m_events.begin() + static_cast<int>(pair.offIndex));
                m_events.erase(m_events.begin() + static_cast<int>(pair.onIndex));
                insertEvent(onEvent, static_cast<unsigned>(OnAbsoluteTime));
                insertEvent(offEvent, static_cast<unsigned>(OffAbsoluteTime));
            }
        }
    }
    m_noteEvents.clear();
    createNoteEventPairs();
}


NoteHoverState Pattern::findNoteHoverState(noteCoordinate hoverCoordinate) {
    NoteHoverState state = noNoteHover;
    for (const auto& pair: m_noteEvents) {
        const auto& onEvent = m_events.at(pair.onIndex);
        const auto& offEvent = m_events.at(pair.offIndex);
        if (onEvent.getPitch() == hoverCoordinate.pitch) {
            if (onEvent.getAbsoluteTime() <= hoverCoordinate.absoluteTime&&
                offEvent.getAbsoluteTime() >= hoverCoordinate.absoluteTime ) {

                auto centerRegion = offEvent.getAbsoluteTime() - (onEvent.getAbsoluteTime() * 0.2);
                auto hoverDelta = hoverCoordinate.absoluteTime;
                if (hoverDelta <= centerRegion ) {
                    state = NoteCenterHover;
                }
                else {
                    state = NoteEdgeHover;
                }
                break;
            }
        }
    }

    return state;
}