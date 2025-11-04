//
// Created by nathan on 17/10/2025.
//

#include "TrackChunk.h"

TrackChunk::TrackChunk(std::vector<uint8_t> bytes){
    m_trackBytes = bytes;
    createNoteEvents();
}

TrackChunk::~TrackChunk() {

}


void TrackChunk::createNoteEvents() {
    size_t index = 0;
    const size_t size = m_trackBytes.size();

    while (index < size) {
        uint8_t status = m_trackBytes.at(index);

        if (status == 0xFF) { // Meta event
            if (index + 2 >= size) break; // Safety check

            uint8_t meta_type = m_trackBytes[index + 1];
            uint8_t length = m_trackBytes[index + 2];

            // Ensure we have enough bytes to read
            if (index + 3 + length > size) break;

            std::vector<uint8_t> meta_bytes =
                vector_slice(m_trackBytes, index + 3, index + 3 + length);

            MidiEvent metaEvent(meta_type, length, meta_bytes);
            Events.push_back(metaEvent);

            // Handle end-of-track
            if (meta_type == 0x2F)
                return;

            // Advance index by header + length
            index += 3 + length;
        }
        else {
            // TODO: handle MIDI channel events, sysex, etc.
            index++;
        }
    }
}

short TrackChunk::readVariableLength(size_t index) {

    //
    do {
        u_int8_t bytes = m_trackBytes.at(index);
        uint8_t byte1 = (bytes >> 4);
        uint8_t byte2 = bytes & 0x0F;
        DBG(byte1<< byte2);
        if ((m_trackBytes.at(index)>> 7) == 0) {

        }
        index++;
    }
    while ((m_trackBytes.at(index) >> 7) == 1);

}


void TrackChunk::printMidiInfo() {
    DBG( "Bytes :" << vector_to_HexString(m_trackBytes));

}