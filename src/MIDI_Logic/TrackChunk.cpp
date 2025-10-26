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


}

void TrackChunk::printMidiInfo() {
    DBG( "Bytes :" << vector_to_HexString(m_trackBytes));

}