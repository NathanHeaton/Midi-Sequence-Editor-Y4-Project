//
// Created by nathan on 17/10/2025.
//

#ifndef MYPROJECT_PARSEDMIDI_H
#define MYPROJECT_PARSEDMIDI_H
#include <cstdint>
#include <vector>
#include <iostream>
#include  "../utils.h"
#include  "TrackChunk.h"

class ParsedMidi{
    public:
    ParsedMidi(const std::vector<uint8_t>& midi_bytes, std::string title);
    ~ParsedMidi();

    //vars
    std::vector<uint8_t> m_bytes;
    int MIDI_FORMAT;
    size_t num_of_tracks;
    bool SMPTE_time;
    std::vector<uint8_t> m_headerChunkLength;
    std::string m_title;

    // track chunk
    std::vector<TrackChunk> m_tracks;

    bool isTrackChunk;
    std::vector<int> track_start;
    std::vector<int> trackLength;


    int ticksInQuarterNote;
    const std::string CHUNK_TITLE_HEX = "4D54726B";
    const int HEADER_END = 14;
    std::string printMidiInfo();

    private:

    void createMidiChunk(int trackStart);
    bool validateTrackChunk(int trackStart);
    void establishSMPTE();

    void get_length_of_tracks();
    int check_if_next_track_valid(int byte, int track_length);

};


#endif //MYPROJECT_PARSEDMIDI_H