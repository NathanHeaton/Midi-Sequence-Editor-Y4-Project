//
// Created by nathan on 17/10/2025.
//

#include "ParsedMidi.h"



ParsedMidi::ParsedMidi(const std::vector<uint8_t>& midi_bytes) {
    m_bytes = midi_bytes;
    MIDI_FORMAT = vector_bytes_to_int(vector_slice(m_bytes,8,9));
    tracks =  vector_bytes_to_int(vector_slice(m_bytes,10,11));
    SMPTE_time = false;
    ticksInQuarterNote = vector_bytes_to_int(vector_slice(m_bytes,12,13));;
    m_headerChunkLength = vector_slice(m_bytes,4,7);
    printMidiInfo();

    if (MIDI_FORMAT == 0)
    {
        validateTrackChunk(HEADER_END);
    }
    else if (MIDI_FORMAT == 1) {

    }
    else {

    }
}

ParsedMidi::~ParsedMidi() {
};

void ParsedMidi::createMidiChunk(int track) {
    int length = vector_bytes_to_int(vector_slice(m_bytes,track,track+3));
    DBG(vector_to_HexString(vector_slice(m_bytes,track,track+3)));
    trackLength.insert(trackLength.begin(),length);
    DBG("length: " << length );

}

void ParsedMidi::validateTrackChunk(int trackStart) {

    if (vector_to_HexString(vector_slice(m_bytes,trackStart,trackStart+3)) == CHUNK_TITLE_HEX ) {
        createMidiChunk(trackStart+4);
    }
    else {
        DBG(vector_to_HexString(vector_slice(m_bytes,trackStart,trackStart+3)));
        DBG("Wrong track chunk size");
    }
}


void ParsedMidi::establishSMPTE() {

}
void ParsedMidi::printMidiInfo() {
    DBG( "MIDI_FORMAT :" << MIDI_FORMAT);
    DBG( "Tracks :" << tracks);
    DBG( "smpte :" << std::to_string(SMPTE_time));
    DBG( "ticksInQuarterNote:" << ticksInQuarterNote);
    DBG( "m_headerChunkLength :" << vector_bytes_to_int(m_headerChunkLength)<< " contents :"<< vector_to_HexString(m_headerChunkLength));
    DBG( "divisions :" << ticksInQuarterNote);
}