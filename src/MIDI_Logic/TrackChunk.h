//
// Created by nathan on 17/10/2025.
//

#ifndef MYPROJECT_TRACKCHUNK_H
#define MYPROJECT_TRACKCHUNK_H
#include <cstdint>
#include <vector>
#include <juce_core/juce_core.h>
#include "MIDI_Events/MidiEvent.h"
#include "../utils.h"

class TrackChunk {
    public:
    TrackChunk(std::vector<uint8_t> bytes);
    ~TrackChunk();
    void printMidiInfo();
    void createNoteEvents();
    short readVariableLength(size_t index);

    std::vector<MidiEvent> Events;


    std::vector<uint8_t> m_trackBytes;

    private:
};


#endif //MYPROJECT_TRACKCHUNK_H