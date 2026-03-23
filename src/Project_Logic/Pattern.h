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

struct NoteCoordinate;
struct SelectionCoords;
struct NoteMoveDelta;
/*====================================
 * Stores of and on ID of events pairs
 ==================================*/
struct NoteEventPair {
    uint32_t onID;
    uint32_t offID;
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
    void insertEvent(MidiEvent& event, uint32_t absoluteTime);
    void addNote(uint8_t t_pitch, uint32_t absoluteTime, uint32_t endDelta, uint32_t t_id[]);
    void removeNote(NoteEventPair notePair );

    void removeNoteOperation(NoteEventPair notepair);
    void timeShiftOperation(uint32_t t_timeDelta, std::unordered_set<uint32_t> IDs);

    uint32_t calculateDelta(size_t insertionIndex, uint32_t absoluteTime)const;

    void calculateSelection(const SelectionCoords &t_selection);
    void pitchShiftSelection(signed short t_pitchDelta);
    void timeShiftSelection(int32_t t_timeDelta);
    void removeSelection(std::vector<NoteCoordinate> t_events);
    void moveNoteEventSelection(NoteMoveDelta coordinateDelta);
    void stretchNoteEventSelection(NoteMoveDelta notePair);
    void deleteSelection();

    void stretchNoteEvent(uint32_t ID, int32_t newEndAbsolute);

    void hideNoteEvent(NoteCoordinate coordinate);
    void moveNoteEvent(uint32_t ID, NoteCoordinate coordinateDelta);

    void fixDeltaFromDeletedNote(NoteEventPair* pair);
    void showAllNoteEvents() {m_hiddenNoteIDs.clear();}

    void hideNoteByID(uint32_t ID) {m_hiddenNoteIDs.insert(ID);}
    void clearPattern(){
        m_events.clear();
        m_noteEvents.clear();
        m_hiddenNoteIDs.clear();
        m_selectedNoteIDs.clear();
    }

    std::optional<NoteEventPair> findNoteBasedOnPoint(NoteCoordinate noteCoordinate);
    NoteHoverState findNoteHoverState(NoteCoordinate hoverCoordinate);

    // coverts selected on note ids into note events pairs
    [[nodiscard]] std::vector<NoteEventPair> convertNoteIdsToNotePair() {
        std::vector<NoteEventPair> events;
        for (const auto selectionID: m_selectedNoteIDs) {
            for (const auto& pair: m_noteEvents) {
                if (selectionID == pair.onID) {
                    events.push_back(pair);
                }
            }
        }
        return events;
    }

    [[nodiscard]] const MidiEvent *getMidiEventByID_ptr(uint32_t ID) const{
        for (auto& event: m_events) {
            if (event.getID() == ID) {
                return &event;
            }
        }
        return nullptr;
    }

    [[nodiscard]] MidiEvent *getMidiEventByID_ptr(uint32_t ID) {
        for (auto& event: m_events) {
            if (event.getID() == ID) {
                return &event;
            }
        }
        return nullptr;
    }

    [[nodiscard]] size_t getEventIndexByID(uint32_t ID) const{
        for (auto i{0u}; i < m_events.size(); ++i ) {
            if (m_events.at(i).getID() == ID) {
                return i;
            }
        }
        return SIZE_MAX;
    }

    [[nodiscard]] size_t findInsertionPoint(uint32_t absoluteTime);

    [[nodiscard]] const NoteEventPair* getEventIDPairFromOnID(uint32_t ID) const{
        for (auto i{0u}; i < m_noteEvents.size(); ++i ) {
            if (m_noteEvents.at(i).onID == ID) {
                return &m_noteEvents.at(i);
            }
        }
        return nullptr;
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

    void addNoteSelection(std::vector<MidiEvent> t_events) {
        m_events.insert(m_events.end(), t_events.begin(), t_events.end());
    }

    NoteEventPair findPairByID(uint32_t id) {
        for (auto& pair : m_noteEvents) {
            if (pair.onID == id)
                return pair;
        }
        std::cout << "NoteEventPairs not found" << std::endl;
        return NoteEventPair(SIZE_MAX,SIZE_MAX);
    }


};
#endif //MYPROJECT_PATTERN_H