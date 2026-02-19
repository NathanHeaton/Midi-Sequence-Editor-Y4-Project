#include "Pattern.h"

#include <algorithm>

#include "../Singletons/SessionData.h"
#include <iostream>
#include <cmath>
#include "../Singletons/ToolManager.h"
#include "../Singletons/PatternManager.h"
//
// Created by nathan on 11/01/2026.
//

void Pattern::setLastBar() {
    const auto barSize = SessionData::instance().getPPQ() * SessionData::instance().timeSignature.getNumerator();
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
            SessionData::instance().getPPQ());
        event.setDelta(newDelta);
    }
}

void Pattern::removeNote(uint8_t t_pitch, uint32_t absoluteTime) {
    if (m_events.empty()) {
        return;
    }
    for (auto i{0u};i<m_noteEvents.size();i++){
        auto& note = m_noteEvents.at(i);
        auto& onNote = m_events.at(note.onIndex);
        auto& offNote = m_events.at(note.offIndex);
        if (onNote.m_absoluteTime < absoluteTime &&
            offNote.m_absoluteTime > absoluteTime) {
            if (onNote.getPitch() == t_pitch) {
                auto onDelta = onNote.getDelta();
                if (note.onIndex + 1 < m_events.size()) {
                    m_events[note.onIndex + 1].setDelta(m_events[note.onIndex + 1].getDelta() +onDelta );
                }

                auto offDelta = offNote.getDelta();
                if (note.offIndex + 1 < m_events.size()) {
                    m_events[note.offIndex + 1].setDelta(m_events[note.offIndex + 1].getDelta() +offDelta );
                }
                //for m_events
                m_events.erase(m_events.begin() + note.offIndex);
                m_events.erase(m_events.begin() + note.onIndex);
                break;
            }
            }
    }
}

void Pattern::removeSelection(noteCoordinate event) {
    removeNote(event.pitch,event.absoluteTime);
    m_noteEvents.clear();
    createNoteEventPairs();
}

void Pattern::removeSelection(std::vector<noteCoordinate> events ) {
    for (auto it{events.begin()};it< events.end();it++) {
        removeNote(it->pitch, it->absoluteTime);
    }
    m_noteEvents.clear();
    createNoteEventPairs();
}

void Pattern::calculateSelection(const SelectionCoords &t_selection) {
    m_selectedNoteIDs.clear();

    float pixelPerTick = SessionData::instance().getPPQ() / SessionData::instance().getPixelPerBeat(zoomFactor::pianoRoll);
    float noteHeight = SessionData::instance().getNoteHeight();

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

void Pattern::addNote(uint8_t t_pitch, uint32_t absoluteTime, uint32_t endDelta, uint32_t t_id) {
        uint8_t channel = 0;
        uint8_t velocity = 127;
        uint32_t onDelta = 0;
        uint32_t offAbsoluteTime = absoluteTime + endDelta;
        size_t startNoteI = 0;
        size_t endNoteI = 0;
        if (m_events.empty()) {
            onDelta = absoluteTime;
        }
        else {
            for (startNoteI = 0; startNoteI < m_events.size(); startNoteI++) {
                if (m_events[startNoteI].getAbsoluteTime() > absoluteTime) {
                    break; // first note
                }
            }
            if (startNoteI == 0) {
                onDelta = absoluteTime;
                m_events[startNoteI].setDelta(m_events[startNoteI].getAbsoluteTime() - absoluteTime);
            } else {
                onDelta = absoluteTime - m_events[startNoteI - 1].getAbsoluteTime();
                if (startNoteI < m_events.size()) {
                    m_events[startNoteI].setDelta(
                        m_events[startNoteI].getAbsoluteTime() - absoluteTime
                    );
                }
            }
        }
        MidiEvent noteOn(onDelta,0x90,Note{t_pitch,velocity},channel);
        noteOn.m_absoluteTime = absoluteTime;
        noteOn.setID(t_id);
        m_events.insert(m_events.begin()+startNoteI,noteOn);
        for (endNoteI = 0; endNoteI < m_events.size(); endNoteI++) {
            if (m_events[endNoteI].m_absoluteTime > offAbsoluteTime) {
                break;
            }
        }
        if (endNoteI == 0) {
            endDelta = offAbsoluteTime;
            m_events[0].setDelta(m_events[0].m_absoluteTime - offAbsoluteTime);
        }
        else {
            endDelta = offAbsoluteTime - m_events[endNoteI - 1].m_absoluteTime;
            if (endNoteI < m_events.size()) {
                m_events[endNoteI].setDelta(
                    m_events[endNoteI].m_absoluteTime - offAbsoluteTime
                );
            }
        }

        MidiEvent noteOff(endDelta,0x80,Note{t_pitch,velocity},channel);

        noteOff.m_absoluteTime = absoluteTime + endDelta;
        m_events.insert(m_events.begin()+endNoteI,noteOff);
        //TODO: change later to add specific note instead of recaluculating
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

void Pattern::timeShiftSelection(uint32_t t_timeDelta) {


}