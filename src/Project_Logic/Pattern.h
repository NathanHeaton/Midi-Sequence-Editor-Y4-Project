//
// Created by nathan on 04/01/2026.
//

#ifndef MYPROJECT_PATTERN_H
#define MYPROJECT_PATTERN_H

#include <iostream>
#include <string>
#include <unordered_set>

#include "../MIDI_Logic/MIDI_Events/MidiEvent.h"

enum NoteHoverState : int;
class SessionData;

struct noteCoordinate;
struct SelectionCoords;

struct NoteEventPair {
    size_t onIndex;
    size_t offIndex;
    bool hidden = false;
};

class Pattern {
public:
    float m_barLength;
    std::string m_title;

    std::vector<MidiEvent> m_events;
    std::vector<NoteEventPair> m_noteEvents;
    int ticksInMidiFile{0};
    int m_bars{8};

    std::unordered_set<uint32_t> m_selectedNoteIDs;
    std::unordered_set<uint32_t> m_hiddenNoteIDs;

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
    void calculateSelection(const SelectionCoords &t_selection);
    void pitchShiftSelection(signed short t_pitchDelta);
    void timeShiftSelection(int32_t t_timeDelta);
    void insertEvent(MidiEvent& event, uint32_t absoluteTime);
    void addNote(uint8_t t_pitch, uint32_t absoluteTime, uint32_t endDelta, uint32_t t_id);
    void removeNote(NoteEventPair notePair );
    uint32_t calculateDelta(size_t insertionIndex, uint32_t absoluteTime)const;
    void removeSelection(noteCoordinate t_event);
    void removeSelection(std::vector<noteCoordinate> t_events);
    void deleteSelection();

    void hideNoteEvent(noteCoordinate coordinate);

    void showAllNoteEvents() {m_hiddenNoteIDs.clear();}

    NoteEventPair findNoteBasedOnPoint(noteCoordinate noteCoordinate);
    NoteHoverState findNoteHoverState(noteCoordinate hoverCoordinate);

    // coverts selected on note ids into note events pairs
    [[nodiscard]] std::vector<NoteEventPair> convertNoteIdsToNotePair() {
        std::vector<NoteEventPair> events;
        for (const auto selectionID: m_selectedNoteIDs) {
            for (const auto& pair: m_noteEvents) {
                auto onEvent = m_events.at(pair.onIndex);
                if (selectionID == onEvent.getID()) {
                    events.push_back(pair);
                }
            }
        }
        return events;
    }



    [[nodiscard]] size_t findInsertionPoint(uint32_t absoluteTime);


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

    NoteEventPair* findPairByID(uint32_t id) {
        for (auto& pair : m_noteEvents) {
            if (m_events.at(pair.onIndex).getID() == id)
                return &pair;
        }
        return nullptr;
    }


};
#endif //MYPROJECT_PATTERN_H