//
// Created by nathan on 04/01/2026.
//
#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>
#include "../Singletons/TimeData.h"
#include "../MIDI_Logic/MIDI_Events/MidiEvent.h"

enum HoverState : int;
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

struct PitchRange {
    uint8_t lowest  = 127;
    uint8_t highest = 0;

    uint8_t getPitchRange() {
        return highest - lowest;
    }
};



class Pattern {
public:
    PitchRange m_pitchRange;
    std::function<uint32_t()> assignID;
    float m_barLength;
    // make sure to add code to update when time sig changes
    uint32_t barSizeTicks =TimeData::instance().PPQ * TimeData::instance().timeSignature.getNumerator();
    std::string m_title;
    uint32_t ID;
    std::vector<MidiEvent> m_events;
    std::unordered_map<uint32_t, size_t> m_eventsIndex;

    std::vector<NoteEventPair> m_noteEvents;
    std::unordered_map<uint32_t, size_t> m_noteEventsIndex;

    int ticksInMidiFile{0};
    int m_bars{1};

    std::unordered_set<uint32_t> m_selectedNoteOnIDs;
    std::unordered_set<uint32_t> m_hiddenNoteOnIDs;
    std::unordered_set<uint32_t> m_previousOperationsOnNoteIDs;

    Pattern(std::string t_title) {
        m_events.reserve(512);
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
    void addNote(uint8_t t_pitch, uint32_t absoluteTime, uint32_t endDelta);
    void updateBarCount(uint32_t endAbsolute);
    void removeNote(NoteEventPair notePair );
    void scaleNoteEventSelection(float scale);
    void removeNoteOperation(NoteEventPair notepair);
    void timeShiftOperation(uint32_t t_timeDelta, std::unordered_set<uint32_t> IDs);
    uint32_t adjustTimeIfNeeded(MidiEvent& event, uint32_t absoluteTime);
    uint32_t calculateDelta(size_t insertionIndex, uint32_t absoluteTime)const;



    void calculateSelection(const SelectionCoords &t_selection);
    void pitchShiftSelection(signed short t_pitchDelta);
    void timeShiftSelection(int32_t t_timeDelta);
    void removeSelection(std::vector<NoteCoordinate> t_events);
    void moveNoteEventSelection(NoteMoveDelta coordinateDelta);
    void stretchNoteEventSelection(NoteMoveDelta notePair);
    void deleteSelection();
    void pasteClipboard(std::vector<MidiEvent>& copied_events, NoteMoveDelta offSet);
    void setNoteVelocity(NoteEventPair noteID,uint8_t velocity);
    void stretchNoteEvent(uint32_t ID, int32_t newEndAbsolute);

    void hideNoteEvent(NoteCoordinate coordinate);
    void moveNoteEvent(uint32_t ID, NoteCoordinate coordinateDelta);

    void fixDeltaFromDeletedNote(NoteEventPair* pair);
    void showAllNoteEvents() {m_hiddenNoteOnIDs.clear();}

    void updatePitchRange();
    void hideNoteByID(uint32_t ID) {m_hiddenNoteOnIDs.insert(ID);}
    void clearPattern(){
        m_events.clear();
        m_noteEvents.clear();
        m_hiddenNoteOnIDs.clear();
        m_selectedNoteOnIDs.clear();
    }

    std::optional<NoteEventPair> findNoteBasedOnPoint(NoteCoordinate noteCoordinate);
    HoverState findNoteHoverState(NoteCoordinate hoverCoordinate);

    void rebuildNoteIndices() {
        m_eventsIndex.clear();
        m_eventsIndex.reserve(m_events.size());
        m_noteEventsIndex.clear();
        m_noteEventsIndex.reserve(m_noteEvents.size());

        for (size_t i{0}; i < m_events.size(); i++)  {
            m_eventsIndex[m_events[i].getID()] = i;
        }
        for (size_t i{0}; i < m_noteEvents.size(); i++)  {
            m_noteEventsIndex[m_noteEvents[i].onID] = i;
        }
    }

    // coverts selected on note ids into note events pairs
    [[nodiscard]] std::vector<NoteEventPair> convertNoteIdsToNotePair() {
        std::vector<NoteEventPair> events;
        for (const auto selectionID: m_selectedNoteOnIDs) {
            for (const auto& pair: m_noteEvents) {
                if (selectionID == pair.onID) {
                    events.push_back(pair);
                }
            }
        }
        return events;
    }

    [[nodiscard]] const MidiEvent *getMidiEventByID_ptr(uint32_t ID) const{
        auto i = m_eventsIndex.find(ID);
        if (i == m_eventsIndex.end()) {return nullptr;}
        return &m_events[i->second];
    }

    [[nodiscard]] MidiEvent *getMidiEventByID_ptr(uint32_t ID) {
        auto i = m_eventsIndex.find(ID);
        if (i == m_eventsIndex.end()) {return nullptr;}
        return &m_events[i->second];
    }

    [[nodiscard]] MidiEvent *getMidiEventByIDLinearSearch_ptr(uint32_t ID) {
        for (auto& n :m_events) {
            if (n.getID() == ID) return &n;
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
        auto i = m_noteEventsIndex.find(ID);

        if (i == m_noteEventsIndex.end()) {return nullptr;}
        return &m_noteEvents[i->second];
    }

    [[nodiscard]] const NoteEventPair* getEventIDPairFromOffID(uint32_t ID) const{
        auto i = m_noteEventsIndex.find(ID);
        if (i == m_noteEventsIndex.end()) {return nullptr;}
        return &m_noteEvents[i->second];
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
    // ============================================================
    //  Core operation executor
    // ============================================================

    // Snapshots the given pairs, removes them, applies a caller-supplied
    // transform to each (on, off) pair, then re-inserts and validates.
    // All note-mutating operations should go through here.
    template<typename Transform>
    void executeOperation(std::vector<NoteEventPair> pairs, Transform transform) {
        std::cout<<"begun operation"<<std::endl;
        m_previousOperationsOnNoteIDs.clear();

        std::vector<std::pair<MidiEvent, MidiEvent>> snapshots;
        snapshots.reserve(pairs.size());

        for (auto& pair : pairs) {
            snapshots.emplace_back(*getMidiEventByID_ptr(pair.onID),
                                   *getMidiEventByID_ptr(pair.offID));
            removeNoteOperation(pair);
        }

        m_noteEvents.clear();
        createNoteEventPairs();

        for (auto& [on, off] : snapshots) {
            transform(on, off);
            insertEvent(on,  on.getAbsoluteTime());
            insertEvent(off, off.getAbsoluteTime());
            m_previousOperationsOnNoteIDs.insert(on.getID());
        }

        m_noteEvents.clear();
        createNoteEventPairs();
        overlapValidate();
        std::cout<<"ended operation"<<std::endl;
    }


    // ============================================================
    //  Overlap validation
    // ============================================================

    // After any mutation, truncates earlier notes that overlap with
    // newly inserted ones. Loops until the pattern is stable.

    void overlapValidate() {
        std::cout << "Overlap validation begin" << std::endl;
        bool foundOverlap = true;
        while (foundOverlap) {
            foundOverlap = false;
            for (auto onId: m_previousOperationsOnNoteIDs) {
                auto pairA = getEventIDPairFromOnID(onId);
                auto* onA  = getMidiEventByID_ptr(pairA->onID);
                auto* offA = getMidiEventByID_ptr(pairA->offID);
                if (!onA || !offA) continue;
                continue;// for test
                for (size_t j = 0; j < m_noteEvents.size(); j++) {
                    //if (i == j) continue;
                    auto& pairB = m_noteEvents[j];
                    auto* onB  = getMidiEventByID_ptr(pairB.onID);
                    auto* offB = getMidiEventByID_ptr(pairB.offID);
                    if (!onB || !offB) continue;
                    if (onA->getPitch() != onB->getPitch()) continue;

                    uint32_t startA = onA->getAbsoluteTime();
                    uint32_t startB = onB->getAbsoluteTime();
                    uint32_t endB   = offB->getAbsoluteTime();

                    if (startA > startB && startA < endB) {
                        int32_t delta = static_cast<int32_t>(startA) - static_cast<int32_t>(endB);
                        stretchNoteEvent(pairB.onID, delta);
                        foundOverlap = true;
                        goto next_pass;
                    }
                }
            }
            next_pass:;
        }
        m_previousOperationsOnNoteIDs.clear();
    }

};