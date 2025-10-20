//
// Created by nathan on 17/10/2025.
//

#include "ParsedMidi.h"



ParsedMidi::ParsedMidi(const std::vector<uint8_t>& midi_bytes) {
    m_bytes = midi_bytes;
    MIDI_FORMAT = m_bytes.at(8);
    tracks =  m_bytes.at(9);
    SMPTE_time = false;
    ticksInQuarterNote = 0;
    printMidiInfo();
    std::vector<uint8_t> var = vector_slice(m_bytes,8,9999);

}

ParsedMidi::~ParsedMidi() {

}

void ParsedMidi::createMidiChunk() {


}


void ParsedMidi::establishSMPTE() {

}
void ParsedMidi::printMidiInfo() {
    DBG( "MIDI_FORMAT :" << MIDI_FORMAT);
    DBG( "Tracks :" << tracks);
    DBG( "smpte :" << std::to_string(SMPTE_time));
    DBG( "divisions :" << ticksInQuarterNote);
}