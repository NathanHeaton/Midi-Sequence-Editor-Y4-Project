//
// Created by nathan on 17/10/2025.
//

#include "ParsedMidi.h"



ParsedMidi::ParsedMidi(const std::vector<uint8_t>& midi_bytes) {
    m_bytes = midi_bytes;

    MIDI_FORMAT = vector_bytes_to_int(vector_slice(m_bytes,8,9));
    num_of_tracks =  vector_bytes_to_int(vector_slice(m_bytes,10,11));
    SMPTE_time = false;
    ticksInQuarterNote = vector_bytes_to_int(vector_slice(m_bytes,12,13));
    m_headerChunkLength = vector_slice(m_bytes,4,7);


    get_length_of_tracks();
    // if (MIDI_FORMAT == 0)
    // {
    //     validateTrackChunk(HEADER_END);
    //     track_start.push_back(HEADER_END);
    // }
    // else if (MIDI_FORMAT == 1) {
    //     get_length_of_tracks();
    // }
    // else {
    //
    // }
}

ParsedMidi::~ParsedMidi() {
};

void ParsedMidi::createMidiChunk(int track) {
    int length = vector_bytes_to_int(vector_slice(m_bytes,track,track+3));
    DBG(vector_to_HexString(vector_slice(m_bytes,track,track+3)));
    trackLength.insert(trackLength.begin(),length);
    DBG("length: " << length );
}


bool ParsedMidi::validateTrackChunk(int trackStart) {

    if (vector_to_HexString(vector_slice(m_bytes,trackStart,trackStart+3)) == CHUNK_TITLE_HEX ) {
        return true;
    }
    else {
        DBG(vector_to_HexString(vector_slice(m_bytes,trackStart,trackStart+3)));
        DBG("Wrong track chunk size");
        return false;
    }
}


void ParsedMidi::get_length_of_tracks() {

    int byte = HEADER_END+4;

    for (int i = 0; i < num_of_tracks; i++) {
        std::vector<uint8_t> slice = vector_slice(m_bytes,byte,byte+3);
        DBG(vector_to_HexString(slice));
        int length = vector_bytes_to_int(slice);
        trackLength.push_back(length);
        DBG("track "<<i<<" length: " << length);

        std::vector<uint8_t> chunkBytes = vector_slice(m_bytes,byte+4,byte+3+length);

        TrackChunk newTrackChunk(chunkBytes);
        m_tracks.push_back(newTrackChunk);
        // move to next byte
        byte = check_if_next_track_valid(byte,length);
    }
}

int ParsedMidi::check_if_next_track_valid(int byte,int track_length) {
    if (byte + track_length + 8 < m_bytes.size()  ) {
        byte = byte + track_length + 4;
        if (validateTrackChunk(byte)) {
            DBG("next chunk is valid");
            byte = byte + 4;
        }
        DBG("byte: "<<byte);
        DBG(vector_to_HexString(vector_slice(m_bytes,byte,byte+3)));
    }
    else if (byte + track_length <= m_bytes.size()  ) {
        DBG("EOF");
    }
    else {
        DBG("accessing byte"<<byte+track_length);
        DBG("about to go out side of file bounds");
    }
    return byte;
}


void ParsedMidi::establishSMPTE() {

}
std::string ParsedMidi::printMidiInfo() {
    std::string allInfo;
    for (TrackChunk i : m_tracks) {

        allInfo += i.printMidiInfo();
    }
    return allInfo;
}