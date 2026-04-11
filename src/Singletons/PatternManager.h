//
// Created by idad on 10/02/2026.
//
#pragma once

#include <vector>
#include "../Project_Logic/Pattern.h"
#include "../MIDI_Logic/ParsedMidi.h"
#include "ToolManager.h"
#include "../GridStructs.h"

enum HoverState: int{
    NoHover,
    EdgeHover,
    CenterHover
};

class PatternManager {
public:

    static PatternManager& instance(){
        static PatternManager singleton;
        return singleton;
    }

    void addPattern() {
        std::string defaultTitle = "unamed_" + std::to_string(unnamedPatterns);
        auto& p = pattern.emplace_back(Pattern(defaultTitle));
        p.assignID = [this]() { return assignNoteId(); };
        p.ID = assignPatternId();
        activePatternId = p.ID;
        unnamedPatterns++;
    }

    Pattern* getPatternByID(uint32_t ID) {
        for (auto& p : pattern) {
            if (p.ID == ID) { return &p; }
        }
        return nullptr;
    }

    void addPatternFromMidi(std::string title, auto& events, int fileTicks) {
        auto& p = pattern.emplace_back(title, events, fileTicks);
        p.assignID = [this]() { return assignNoteId(); };
        p.ID = assignPatternId();
    }

    bool anyPatterns() {
        return !pattern.empty();
    }

    const std::vector<Pattern>& getPatterns() {
        return pattern;
    }

    const Pattern* getCurrentPattern() {
        return getPatternByID(activePatternId);
    }

    void setActivePatternID(uint32_t patternID) {activePatternId = patternID;}
    uint32_t getActivePatternID() {return activePatternId;}

    size_t getPatternSize() { return pattern.size(); }

    // ----------------------------------------------------------------
    // Mutation methods
    // ----------------------------------------------------------------

    void pitchShiftSelection(uint32_t patternID, signed short t_pitchDelta) {
        patternAt(patternID).pitchShiftSelection(t_pitchDelta);
    }

    void timeShiftSelection(uint32_t patternID, int32_t t_timeDelta) {
        patternAt(patternID).timeShiftSelection(t_timeDelta);
    }

    void deleteSelection(uint32_t patternID) {
        patternAt(patternID).deleteSelection();
    }

    void addNoteToPattern(uint32_t patternID, uint8_t t_pitch, int absoluteTime, int endDelta) {
        patternAt(patternID).addNote(t_pitch, absoluteTime, endDelta);
    }

    void removeNoteFromPattern(uint32_t patternID, NoteCoordinate noteCoordinate) {
        std::vector<NoteCoordinate> coords;
        coords.push_back(noteCoordinate);
        patternAt(patternID).removeSelection(coords);
    }

    void moveSelection(uint32_t patternID, NoteMoveDelta coordinate) {
        patternAt(patternID).moveNoteEventSelection(coordinate);
    }

    HoverState getNoteHoverState(uint32_t patternID, NoteCoordinate hoveredCoordinate) {
        return patternAt(patternID).findNoteHoverState(hoveredCoordinate);
    }

    void hideNoteEvent(uint32_t patternID, NoteCoordinate noteCoordinate) {
        patternAt(patternID).hideNoteEvent(noteCoordinate);
    }

    void hideNoteEventByID(uint32_t patternID, uint32_t noteID) {
        patternAt(patternID).hideNoteByID(noteID);
    }

    void showAllEvents(uint32_t patternID) {
        patternAt(patternID).showAllNoteEvents();
    }

    NoteEventPair getNoteEventPairFromCoordinate(uint32_t patternID, NoteCoordinate noteCoordinate) {
        return *patternAt(patternID).findNoteBasedOnPoint(noteCoordinate);
    }

    void setSelection(uint32_t patternID, const SelectionCoords& t_selection) {
        patternAt(patternID).calculateSelection(t_selection);
    }

    bool areNotesSelected(uint32_t patternID) {
        return !patternAt(patternID).m_selectedNoteOnIDs.empty();
    }

    void scaleSelection(uint32_t patternID, float s) {
        patternAt(patternID).scaleNoteEventSelection(s);
    }

    void clearPattern(uint32_t patternID) {
        patternAt(patternID).clearPattern();
    }

    void stretchNoteEvent(uint32_t patternID, uint32_t noteID, int32_t newEndDeltaOffset) {
        ToolManager::instance().setLastNoteDuration(
            ToolManager::instance().getLastNoteDuration() + newEndDeltaOffset);
        patternAt(patternID).stretchNoteEvent(noteID, newEndDeltaOffset);
    }

    void stretchSelection(uint32_t patternID, NoteMoveDelta offsetDelta) {
        patternAt(patternID).stretchNoteEventSelection(offsetDelta);
    }

    void moveNoteEvent(uint32_t patternID, uint32_t noteID, NoteCoordinate coordinatePosition) {
        patternAt(patternID).moveNoteEvent(noteID, coordinatePosition);
    }

    void setNoteVelocity(uint32_t patternID, NoteEventPair noteIDs, int32_t velocity) {
        patternAt(patternID).setNoteVelocity(noteIDs, velocity);
    }

    // ----------------------------------------------------------------
    // Clipboard
    // ----------------------------------------------------------------

    void copyEventsSelectedEvents(uint32_t patternID) {
        if (!clipBoard.empty()) { clipBoard.clear(); }
        auto& p = patternAt(patternID);
        for (auto id : p.m_selectedNoteOnIDs) {
            auto pair = p.getEventIDPairFromOnID(id);
            clipBoard.push_back(*p.getMidiEventByID_ptr(pair->onID));
            clipBoard.push_back(*p.getMidiEventByID_ptr(pair->offID));
        }
    }

    void pasteEvents(uint32_t patternID) {
        if (clipBoard.empty()) { return; }
        patternAt(patternID).pasteClipboard(clipBoard, NoteMoveDelta(0, 0));
    }

    void pasteEventsOnMouse(uint32_t patternID, NoteCoordinate snappedCoordinate) {
        if (clipBoard.empty()) { return; }
        NoteCoordinate topLeftEvent(clipBoard.at(0).getPitch(), clipBoard.at(0).getAbsoluteTime());
        for (const auto& event : clipBoard) {
            if (event.getAbsoluteTime() < topLeftEvent.absoluteTime) {
                topLeftEvent.absoluteTime = event.getAbsoluteTime();
                topLeftEvent.pitch = event.getPitch();
            }
        }
        NoteMoveDelta offset(snappedCoordinate.pitch - topLeftEvent.pitch,
            static_cast<signed>(snappedCoordinate.absoluteTime - topLeftEvent.absoluteTime));
        patternAt(patternID).pasteClipboard(clipBoard, offset);
    }

    // ----------------------------------------------------------------
    // MIDI import
    // ----------------------------------------------------------------

    void addParsedMidi(auto& data, std::string title) {
        parsedMidiFile.emplace_back(data, title);
        updatePatternWithMidiData();
    }

    void setCurrentPattern(size_t newPattern) {
        activePatternIndex = newPattern;
        activePatternId = patternAt(newPattern).ID;
    }

    uint32_t assignNoteId()    { return m_nextNoteId++; }
    uint32_t assignPatternId() { return m_nextPatternId++; }



private:
    PatternManager() = default;

    std::vector<MidiEvent> clipBoard;
    std::vector<Pattern>   pattern{};
    u_int unnamedPatterns = 0;
    size_t activePatternIndex = 0;
    uint32_t activePatternId = 0;

    std::vector<ParsedMidi> parsedMidiFile;

    uint32_t m_nextNoteId{0};
    uint32_t m_nextPatternId{0};

    Pattern& patternAt(uint32_t id) {
        for (auto& p : pattern) {
            if (p.ID == id) { return p; }
        }
        throw std::out_of_range("PatternManager::patternAt – unknown pattern ID");
    }

    void updatePatternWithMidiData() {
        ParsedMidi& currentFile = parsedMidiFile.back();

        size_t tracks = currentFile.m_tracks.size();
        if (currentFile.MIDI_FORMAT == 0) {
            std::vector<MidiEvent> combinedTracks;
            auto title = currentFile.m_title + " ";
            for (size_t track = 0; track < tracks; track++) {
                assignIdsToMidi(currentFile.m_tracks.at(track).Events);
                combinedTracks.insert(combinedTracks.end(),
                    currentFile.m_tracks.at(track).Events.begin(),
                    currentFile.m_tracks.at(track).Events.end());
            }
            addPatternFromMidi(title, combinedTracks, currentFile.ticksInQuarterNote);
        }
        else {
            loopThroughTracks(currentFile);
        }
    }

    void assignIdsToMidi(std::vector<MidiEvent>& events) {
        for (auto& event : events) {
            event.setID(assignNoteId());
        }
    }

    void loopThroughTracks(auto& currentFile) {
        size_t tracks = currentFile.m_tracks.size();
        for (size_t track = 0; track < tracks; track++) {
            auto title = currentFile.m_title + " " + std::to_string(track);
            assignIdsToMidi(currentFile.m_tracks.at(track).Events);
            if (currentFile.m_tracks.at(track).m_noteTrack) {
                addPatternFromMidi(title, currentFile.m_tracks.at(track).Events,
                    currentFile.ticksInQuarterNote);
            }
            else { std::cout << "skipping non note track" << std::endl; }
        }
    }
};