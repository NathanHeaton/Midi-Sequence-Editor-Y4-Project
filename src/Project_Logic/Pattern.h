//
// Created by nathan on 04/01/2026.
//

#ifndef MYPROJECT_PATTERN_H
#define MYPROJECT_PATTERN_H

#include <iostream>
#include <ostream>
#include <string>
#include "../MIDI_Logic/MIDI_Events/MidiEvent.h"

class SessionData;

class Pattern {
public:


    struct NoteEventPair {
        size_t onIndex;
        size_t offIndex;
    };


    float m_barLength;


    std::string m_title;

    std::vector<MidiEvent> m_events;
    std::vector<NoteEventPair> m_noteEvents;
    int ticksInMidiFile{0};
    int m_bars{8};

    Pattern(std::string t_title) {
        m_title = t_title;
    }
    // from Midi import
    Pattern(std::string t_title, std::vector<MidiEvent>& events, int midiTicks)
        : m_events(events), ticksInMidiFile(midiTicks) {
        m_title = t_title;
        convertMidiTicksToPPQ();
        createNoteEventPairs();
        if (!m_events.empty()) {
            setLastBar();
        }
    }
    void createNoteEventPairs();

    void convertMidiTicksToPPQ();

    void setLastBar();


    size_t findInsertionPoint(uint32_t absoluteTime) {
        size_t i = 0;
        for (i; i < m_events.size(); i++) {
            if (m_events.at(i).m_absoluteTime > absoluteTime ) {
                break;
            }
        }
        return i;
    }



    const void addNote(uint8_t t_pitch, uint32_t absoluteTime, uint32_t endDelta) {
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
                if (m_events[startNoteI].m_absoluteTime > absoluteTime) {
                    break; // first note
                }
            }
            if (startNoteI == 0) {
                onDelta = absoluteTime;
                m_events[startNoteI].setDelta(m_events[startNoteI].m_absoluteTime - absoluteTime);
            } else {
                onDelta = absoluteTime - m_events[startNoteI - 1].m_absoluteTime;
                if (startNoteI < m_events.size()) {
                    m_events[startNoteI].setDelta(
                        m_events[startNoteI].m_absoluteTime - absoluteTime
                    );
                }
            }
        }
        MidiEvent noteOn(onDelta,0x90,Note{t_pitch,velocity},channel);
        noteOn.m_absoluteTime = absoluteTime;
        printf("point before insertion");
        m_events.insert(m_events.begin()+startNoteI,noteOn);


        endNoteI = findInsertionPoint(offAbsoluteTime);

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

    void removeNote(uint8_t t_pitch, uint32_t absoluteTime) {
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
                    m_noteEvents.clear();
                    createNoteEventPairs();
                    break;
                }
            }
        }
    }

    void updateEventDeltas(auto startIndex, auto deltaIncrement) {
        // for (int i = startIndex+1; i < m_events.size(); i++) {
        //     m_events.at(i).m_delta += deltaIncrement;
        // }
    }

    void printEvents() {
        printf("Events ==========\n");
        int count{0};
        for (auto event : m_events) {
            printf("Note index %d =======\n", count);
            if (event.isNoteOff()) {
                printf("note is note off\n");
                printf("pitch %d\n",event.getPitch());
            }
            else if (event.isNoteOn()) {
                printf("note is note on\n");
                printf("pitch %d\n",event.getPitch());
            }
            else {
                printf("note is other\n");
            }
            count++;
        }
    }

    void printNotePairs() {
        printf("NotePairs\n");
        for (auto i{0u};i<m_noteEvents.size();i++) {
            printf("pair index %d\n",i);
            printf("note on index %d\n ",m_noteEvents.at(i).onIndex);
            printf("note off index %d\n ",m_noteEvents.at(i).offIndex);
        }
    }

    void addNoteSelection(std::vector<MidiEvent> t_events) {
        m_events.insert(m_events.end(), t_events.begin(), t_events.end());

    }




};
#endif //MYPROJECT_PATTERN_H