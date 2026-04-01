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
        unnamedPatterns++;
    }

    size_t getPatternIndexByID(uint32_t ID) {
        for (auto& p : pattern) {
            if (p.ID == ID){return p.ID;}
        }
        return NULL;
    }

    void addPatternFromMidi(std::string title, auto& events, int fileTicks) {
        pattern.emplace_back(title, events, fileTicks);
    }
    bool anyPatterns() {
        return !pattern.empty();
    }
    const std::vector<Pattern>& getPatterns() {
        return pattern;
    }

    const Pattern& getCurrentPattern() {
        return pattern.at(activePatternIndex);
    }

    size_t getPatternSize() {return pattern.size();}

    void pitchShiftSelection(signed short t_pitchDelta) {
        pattern.at(activePatternIndex).pitchShiftSelection(t_pitchDelta);
    }

    void timeShiftSelection(int32_t t_timeDelta) {
        pattern.at(activePatternIndex).timeShiftSelection(t_timeDelta);
    }

    void deleteSelection() {
        pattern.at(activePatternIndex).deleteSelection();
    }

    void addNoteToPattern(uint8_t t_pitch, int absoluteTime, int endDelta) {
        pattern.at(activePatternIndex).addNote( t_pitch,  absoluteTime,  endDelta);
    }

    void removeNoteFromPattern(NoteCoordinate noteCoordinate ) {
        std::vector<NoteCoordinate> coords;
        coords.push_back(noteCoordinate);
        pattern.at(activePatternIndex).removeSelection(coords);
    }

    void moveSelection(NoteMoveDelta coordinate) {
        pattern.at(activePatternIndex).moveNoteEventSelection(coordinate);
    }

    void setCurrentPattern(size_t newPattern) {
        activePatternIndex = newPattern;
    }

    HoverState getNoteHoverState(NoteCoordinate hoveredCoordinate) {
        return pattern.at(activePatternIndex).findNoteHoverState(hoveredCoordinate);
    }

    void hideNoteEvent(NoteCoordinate noteCoordinate)
    {
        pattern.at(activePatternIndex).hideNoteEvent(noteCoordinate);
    }

    void hideNoteEventByID(uint32_t ID)
    {
        pattern.at(activePatternIndex).hideNoteByID(ID);
    }

    void showAllEvents()
    {
        pattern.at(activePatternIndex).showAllNoteEvents();
    }

    NoteEventPair getNoteEventPairFromCoordinate(NoteCoordinate noteCoordinate)
    {
        return *pattern.at(activePatternIndex).findNoteBasedOnPoint(noteCoordinate);
    }

    void setSelection(const SelectionCoords &t_selection) {
        pattern.at(activePatternIndex).calculateSelection(t_selection);
    }

    bool areNotesSelected(){
        if (pattern.at(activePatternIndex).m_selectedNoteOnIDs.size() > 0)
            {return true;}
        return false;
    }

    void scaleSelection(float s) {
        pattern.at(activePatternIndex).scaleNoteEventSelection(s);
    }

    void clearPattern(){
        pattern.at(activePatternIndex).clearPattern();
    }

    void addParsedMidi(auto& data, std::string title) {
        parsedMidiFile.emplace_back(data,title);
        updatePatternWithMidiData();
    }

    void stretchNoteEvent(uint32_t ID, int32_t newEndDeltaOffset) {
        ToolManager::instance().setLastNoteDuration(
            ToolManager::instance().getLastNoteDuration()
            +newEndDeltaOffset);
        pattern.at(activePatternIndex).stretchNoteEvent(ID,newEndDeltaOffset);
    }

    void stretchSelection(NoteMoveDelta offsetDelta)
    {
        pattern.at(activePatternIndex).stretchNoteEventSelection(offsetDelta);
    }

    void moveNoteEvent(uint32_t ID, NoteCoordinate coordinatePosition)
    {
        pattern.at(activePatternIndex).moveNoteEvent(ID, coordinatePosition);
    }

    //ParsedMidi* currentFile;
    void updatePatternWithMidiData() {
        ParsedMidi& currentFile = parsedMidiFile.at(parsedMidiFile.size()-1);

        size_t tracks = currentFile.m_tracks.size();
        if (currentFile.MIDI_FORMAT == 0) {
            std::vector<MidiEvent> combinedTracks;
            auto title =  currentFile.m_title + " ";
            for (size_t track = 0; track < tracks; track++) {
                assignIdsToMidi(currentFile.m_tracks.at(track).Events);
                combinedTracks.insert(combinedTracks.end(), currentFile.m_tracks.at(track).Events.begin() ,currentFile.m_tracks.at(track).Events.end());
            }
            addPatternFromMidi(title,combinedTracks, currentFile.ticksInQuarterNote);
        }
        else {
            loopThroughTracks(currentFile);
        }
        setCurrentPattern(pattern.size()-1);
    }

    void copyEventsSelectedEvents() {
        if (!clipBoard.empty()){clipBoard.clear();}
        auto& p = getCurrentPattern();
        for ( auto id : p.m_selectedNoteOnIDs){
            auto pair = p.getEventIDPairFromOnID(id);
            clipBoard.push_back(*p.getMidiEventByID_ptr(pair->onID));
            clipBoard.push_back(*p.getMidiEventByID_ptr(pair->offID));
        }
    }

    void pasteEvents() {
        if (clipBoard.empty()){return;}
        pattern.at(activePatternIndex).pasteClipboard(clipBoard, NoteMoveDelta(0,0));
    }

    void pasteEventsOnMouse(NoteCoordinate snappedCoordinate){
        if (clipBoard.empty()){return;}
        NoteCoordinate topLeftEvent(clipBoard.at(0).getPitch(),clipBoard.at(0).getAbsoluteTime());
        for (const auto& event : clipBoard) {
            if (event.getAbsoluteTime() < topLeftEvent.absoluteTime) {
                topLeftEvent.absoluteTime = event.getAbsoluteTime();
                topLeftEvent.pitch = event.getPitch();
            }
        }
        NoteMoveDelta offset(snappedCoordinate.pitch - topLeftEvent.pitch,
             static_cast<signed>(snappedCoordinate.absoluteTime - topLeftEvent.absoluteTime));
        pattern.at(activePatternIndex).pasteClipboard(clipBoard, offset);
    }


    uint32_t assignNoteId() { return m_nextNoteId++; }
    uint32_t assignPatternId() {return m_nextPatternId++;}

    size_t activePatternIndex = 0;
    private:
    PatternManager() = default;

    std::vector<MidiEvent> clipBoard;
    std::vector<Pattern> pattern{};
    u_int unnamedPatterns = 0;

    std::vector<ParsedMidi> parsedMidiFile;

    uint32_t m_nextNoteId{0};
    uint32_t m_nextPatternId{0};

    void assignIdsToMidi(std::vector<MidiEvent>& events) {
        for (auto& event : events) {
            event.setID(assignNoteId());
        }
    }

    void loopThroughTracks(auto& currentFile) {
        size_t tracks = currentFile.m_tracks.size();
        for (size_t track = 0; track < tracks; track++) {
            auto title =  currentFile.m_title + " " + std::to_string(track);
            assignIdsToMidi(currentFile.m_tracks.at(track).Events);

            if (currentFile.m_tracks.at(track).m_noteTrack) {
                addPatternFromMidi(title,currentFile.m_tracks.at(track).Events, currentFile.ticksInQuarterNote);
            }
            else{std::cout << "skipping non note track" << std::endl;}
        }
    }



};