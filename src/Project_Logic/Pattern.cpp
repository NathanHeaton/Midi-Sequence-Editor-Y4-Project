#include "Pattern.h"
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

void Pattern::calculateSelection(SelectionCoords &t_selection) {

}

void Pattern::addNote(uint8_t t_pitch, uint32_t absoluteTime, uint32_t endDelta, uint32_t t_id) {
        uint8_t channel = 0;
        uint8_t velocity = 127;
        uint32_t onDelta = 0;
        uint32_t offAbsoluteTime = absoluteTime + endDelta;
        size_t startNoteI = 0;
        size_t endNoteI = 0;

        printf("check if empty \n ");
        if (m_events.empty()) {
            onDelta = absoluteTime;
        }
        else {
            printf("not empty \n ");
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

        printf("point before insertion");
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
        printf("On Delta: %d absolute of note to insert: %d\n endDeltaOf note: %d\n\n ",onDelta,absoluteTime,endDelta);
        //TODO: change later to add specific note instead of recaluculating
        m_noteEvents.clear();
        createNoteEventPairs();
    }