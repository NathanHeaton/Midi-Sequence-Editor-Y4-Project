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

    const void addNote(uint8_t t_pitch, int absoluteTime, uint32_t endDelta) {
        uint8_t channel = 0;
        uint8_t velocity = 127;
        uint8_t status = 0x80;
        uint32_t delta = 0;
        size_t index = 0;

        for ( index; index < m_events.size(); index++) {
            if (absoluteTime >= m_events.at(index).m_absoluteTime) {
                printf("absoluteTime of previous note: %d absolute of note to insert: %d\n",m_events.at(index).m_absoluteTime,absoluteTime);
                delta = absoluteTime - m_events.at(index).m_absoluteTime;
                break;
            }
        }

        MidiEvent noteOn(delta,status,Note{t_pitch,velocity},channel);
        noteOn.m_absoluteTime = absoluteTime;
        status = 0x90;
        MidiEvent noteOff(endDelta,status,Note{t_pitch,velocity},channel);
        noteOff.m_absoluteTime = absoluteTime + endDelta;
        m_events.insert(m_events.begin()+index,noteOn);
        m_events.insert(m_events.begin()+index,noteOff);

        //TODO: change later to add specific note instead of recaluculating
        createNoteEventPairs();
    }

    void updateEventDeltas() {
        //for ()
    }


    void addNoteSelection(std::vector<MidiEvent> t_events) {
        m_events.insert(m_events.end(), t_events.begin(), t_events.end());

    }


};
#endif //MYPROJECT_PATTERN_H