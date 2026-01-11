//
// Created by nathan on 17/10/2025.
//

#include "ParsedMidi.h"



ParsedMidi::ParsedMidi(const std::vector<uint8_t>& midi_bytes, std::string title) {
    m_bytes = midi_bytes;
    m_title = title;
    MIDI_FORMAT = vector_bytes_to_int(vector_slice(m_bytes,8,9));
    num_of_tracks =  vector_bytes_to_int(vector_slice(m_bytes,10,11));
    SMPTE_time = false;
    ticksInQuarterNote = vector_bytes_to_int(vector_slice(m_bytes,12,13));
    m_headerChunkLength = vector_slice(m_bytes,4,7);

    std::cout << "ParsedMidi 1" << std::endl;
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
    std::cout << "ParsedMidi 2" << std::endl;
    trackLength.insert(trackLength.begin(),length);
}


bool ParsedMidi::validateTrackChunk(int trackStart) {

    if (vector_to_HexString(vector_slice(m_bytes,trackStart,trackStart+3)) == CHUNK_TITLE_HEX ) {
        return true;
    }
    else {
        return false;
    }
}


void ParsedMidi::get_length_of_tracks() {

    int byte = HEADER_END+4;

    for (int i = 0; i < num_of_tracks; i++) {
        std::vector<uint8_t> slice = vector_slice(m_bytes,byte,byte+3);
        int length = vector_bytes_to_int(slice);
        trackLength.push_back(length);

        std::vector<uint8_t> chunkBytes = vector_slice(m_bytes,byte+4,byte+3+length);
        std::cout << "creating track chunk" << std::endl;
        TrackChunk newTrackChunk(chunkBytes);
        std::cout << "created track chunk" << std::endl;
        m_tracks.push_back(newTrackChunk);
        // move to next byte
        byte = check_if_next_track_valid(byte,length);
    }
    std::cout << "finished function" << std::endl;
}

int ParsedMidi::check_if_next_track_valid(int byte,int track_length) {
    if (byte + track_length + 8 < m_bytes.size()  ) {
        byte = byte + track_length + 4;
        if (validateTrackChunk(byte)) {
            byte = byte + 4;
        }
    }
    else if (byte + track_length <= m_bytes.size()  ) {
    }
    else {
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