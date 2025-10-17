//
// Created by nathan on 17/10/2025.
//

#include "ParsedMidi.h"



ParsedMidi::ParsedMidi(const std::vector<uint8_t>& midi_bytes) {
    m_bytes = midi_bytes;
    MIDI_FORMAT = m_bytes.at(8);
    DBG( "MIDI_FORMAT :" << MIDI_FORMAT);

}

ParsedMidi::~ParsedMidi() {

}

void ParsedMidi::createMidiChunk() {

}
