//
// Created by nathan on 17/10/2025.
//

#ifndef MYPROJECT_TRACKCHUNK_H
#define MYPROJECT_TRACKCHUNK_H
#include <cstdint>
#include <vector>
#include <juce_core/juce_core.h>
#include "MIDI_Events/MidiEvent.h"
#include "MidiByteReader.h"
#include "../utils.h"

class TrackChunk {
    public:
    TrackChunk(std::vector<uint8_t>& bytes);
    ~TrackChunk();
    void printMidiInfo();
    void createNoteEvents();
    void handleMetaEvent(uint32_t delta);
    void handleInstrumentEvent(uint32_t delta);
    void handleProgramEvent(uint32_t delta);

    std::vector<MidiEvent> Events;


    const std::vector<uint8_t>& m_trackBytes;
    MidiByteReader m_midiReader;
    private:
    bool m_endofTrack = false;

};


#endif //MYPROJECT_TRACKCHUNK_H