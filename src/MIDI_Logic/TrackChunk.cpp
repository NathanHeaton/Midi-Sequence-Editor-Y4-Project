//
// Created by nathan on 17/10/2025.
//

#include "TrackChunk.h"

#include "juce_graphics/fonts/harfbuzz/OT/Layout/types.hh"

TrackChunk::TrackChunk(std::vector<uint8_t>& bytes) : m_trackBytes(bytes) {
    //createNoteEvents();
}

TrackChunk::~TrackChunk() {

}


void TrackChunk::createNoteEvents() {

    MidiByteReader reader(m_trackBytes);
    while (reader.index < m_trackBytes.size()) {
        uint8_t status = reader.readNext();
        if (status == 0xFF) {
            // Meta event
        }

        //MidiEvent metaEvent(meta_type, length, meta_bytes);
        //Events.push_back(metaEvent);

        // Handle end-of-track

    }
}



void TrackChunk::printMidiInfo() {
    DBG( "Bytes :" << vector_to_HexString(m_trackBytes));

}