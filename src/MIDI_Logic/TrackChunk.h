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

    // META & SYSEX
    void handleMetaEvent(uint32_t delta);
    void handleSysExEvent(uint32_t delta);

    // CHANNEL EVENTS
    void handleNoteOff(uint32_t delta, uint8_t channel);
    void handleNoteOn(uint32_t delta, uint8_t channel);
    void handlePolyAftertouch(uint32_t delta, uint8_t channel);
    void handleControlChange(uint32_t delta, uint8_t channel);
    void handleProgramEvent(uint32_t delta, uint8_t channel);
    void handleChannelAftertouch(uint32_t delta, uint8_t channel);
    void handlePitchBend(uint32_t delta, uint8_t channel);

    std::vector<MidiEvent> Events;


    const std::vector<uint8_t>& m_trackBytes;
    MidiByteReader m_midiReader;
    private:
    bool m_endofTrack = false;

};


#endif //MYPROJECT_TRACKCHUNK_H