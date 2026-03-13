//
// Created by idad on 10/02/2026.
//
#pragma once

#include <vector>
#include "../Project_Logic/Pattern.h"
#include "../MIDI_Logic/ParsedMidi.h"
#include "../NoteStucts.h"

enum NoteHoverState: int{
    noNoteHover,
    NoteEdgeHover,
    NoteCenterHover
};

class PatternManager {
public:

    static PatternManager& instance(){
        static PatternManager singleton;
        return singleton;
    }

    void addPattern() {
        std::string defaultTitle = "unamed_" + std::to_string(unnamedPatterns);
        pattern.emplace_back(Pattern(defaultTitle));
        unnamedPatterns++;
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
        pattern.at(activePatternIndex).addNote( t_pitch,  absoluteTime,  endDelta, assignNoteId());
    }

    void removeNoteFromPattern(noteCoordinate noteCoordinate ) {
        std::vector<struct noteCoordinate> coords;
        coords.push_back(noteCoordinate);
        pattern.at(activePatternIndex).removeSelection(coords);
    }

    void setCurrentPattern(size_t newPattern) {
        activePatternIndex = newPattern;
    }

    NoteHoverState getNoteHoverState(noteCoordinate hoveredCoordinate) {
        return pattern.at(activePatternIndex).findNoteHoverState(hoveredCoordinate);
    }

    void hideNoteEvent(noteCoordinate noteCoordinate)
    {
        pattern.at(activePatternIndex).hideNoteEvent(noteCoordinate);
    }

    NoteEventPair getNoteEventPairFromCoordinate(noteCoordinate noteCoordinate)
    {
        return pattern.at(activePatternIndex).findNoteBasedOnPoint(noteCoordinate);
    }

    void setSelection(const SelectionCoords &t_selection) {
        pattern.at(activePatternIndex).calculateSelection(t_selection);
    }

    void addParsedMidi(auto& data, std::string title) {
        parsedMidiFile.emplace_back(data,title);
        std::cout << "moving onto pattern" << std::endl;
        updatePatternWithMidiData();
    }

    void moveNoteEvent(uint32_t ID, noteCoordinate coordinatePosition, uint32_t endAbsolute)
    {
        pattern.at(activePatternIndex).moveNoteEvent(ID, coordinatePosition, endAbsolute);
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

    private:
    PatternManager() = default;

    std::vector<Pattern> pattern{};
    size_t activePatternIndex = 0;
    u_int unnamedPatterns = 0;

    std::vector<ParsedMidi> parsedMidiFile;

    uint32_t m_nextNoteId{0};
    uint32_t assignNoteId() { return m_nextNoteId++; }

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