//
// Created by nathan on 17/10/2025.
//

#include "TrackChunk.h"

TrackChunk::TrackChunk(std::vector<uint8_t> bytes){
    m_trackBytes = bytes;
}

TrackChunk::~TrackChunk() {

}


void TrackChunk::createNoteEvents() {

    size_t index = 0;
    do {
        if (m_trackBytes.at(index) == 0xFF) { // meta event
            u_int8_t meta_type = m_trackBytes.at(index+1);
            u_int8_t length = m_trackBytes.at(index+2);
            if (meta_type == 0x00) {

            }

        }
        index++;
    }
    while (m_trackBytes.size() > index);
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