//
// Created by nathan on 04/01/2026.
//

#ifndef MYPROJECT_PATTERN_H
#define MYPROJECT_PATTERN_H

#include <iostream>
#include <string>
#include "../MIDI_Logic/MIDI_Events/MidiEvent.h"

class SessionData;

struct noteCoordinate;

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

    void addNote(uint8_t t_pitch, uint32_t absoluteTime, uint32_t endDelta);
    void removeNote(uint8_t t_pitch, uint32_t absoluteTime);

    void removeSelection(noteCoordinate t_event);
    void removeSelection(std::vector<noteCoordinate> t_events);

    size_t findInsertionPoint(uint32_t absoluteTime) {
        size_t i = 0;
        for (i; i < m_events.size(); i++) {
            if (m_events.at(i).m_absoluteTime > absoluteTime ) {
                break;
            }
        }
        return i;
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