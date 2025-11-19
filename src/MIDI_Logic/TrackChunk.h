//
// Created by nathan on 17/10/2025.
//

#ifndef MYPROJECT_TRACKCHUNK_H
#define MYPROJECT_TRACKCHUNK_H
#include <cstdint>
#include <vector>
#include <juce_core/juce_core.h>
#include "MIDI_Events/MidiEvent.h"
#include "juce_events/juce_events.h"
#include "MidiByteReader.h"
#include "../utils.h"
#include "MIDI_Events/MidiStructs.h"

class TrackChunk {
    public:

    TrackChunk(std::vector<uint8_t>& bytes);
    ~TrackChunk();
    std::string printMidiInfo();
    void createNoteEvents();

    // META & SYSEX
    void handleMetaEvent(uint32_t delta, uint8_t status);
    void handleSysExEvent(uint32_t delta, uint8_t status);

    // CHANNEL EVENTS
    void handleNote(uint32_t delta, uint8_t channel, uint8_t status);
    void handlePolyAftertouch(uint32_t delta, uint8_t channel, uint8_t status);
    void handleControlChange(uint32_t delta, uint8_t channel, uint8_t status);
    void handleProgramEvent(uint32_t delta, uint8_t channel, uint8_t status);
    void handleChannelAftertouch(uint32_t delta, uint8_t channel, uint8_t status);
    void handlePitchBend(uint32_t delta, uint8_t channel, uint8_t status);

    std::vector<MidiEvent> Events;


    const std::vector<uint8_t>& m_trackBytes;
    MidiByteReader m_midiReader;
    private:
    bool m_endofTrack = false;

};


#endif //MYPROJECT_TRACKCHUNK_H