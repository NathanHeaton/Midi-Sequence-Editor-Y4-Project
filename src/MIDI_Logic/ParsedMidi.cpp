//
// Created by nathan on 17/10/2025.
//

#include "ParsedMidi.h"



ParsedMidi::ParsedMidi(const std::vector<uint8_t>& midi_bytes) {
    m_bytes = midi_bytes;
    MIDI_FORMAT = bytes_to_int(vector_slice(m_bytes,8,9));
    tracks =  bytes_to_int(vector_slice(m_bytes,10,11));
    SMPTE_time = false;
    ticksInQuarterNote = bytes_to_int(vector_slice(m_bytes,12,13));;
    m_headerChunkLength = vector_slice(m_bytes,4,7);
    printMidiInfo();
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
    DBG( "ticksInQuarterNote:" << ticksInQuarterNote);
    DBG( "m_headerChunkLength :" << bytes_to_int(m_headerChunkLength)<< " contents :"<< vector_to_HexString(m_headerChunkLength));
    DBG( "divisions :" << ticksInQuarterNote);
}