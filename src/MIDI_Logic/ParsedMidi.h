//
// Created by nathan on 17/10/2025.
//

#ifndef MYPROJECT_PARSEDMIDI_H
#define MYPROJECT_PARSEDMIDI_H
#include <cstdint>
#include <vector>
#include <iostream>
#include <juce_core/juce_core.h>
#include "juce_core/system/juce_PlatformDefs.h"
#include  "../utils.h"
#include  "TrackChunk.h"

class ParsedMidi {
    public:
    ParsedMidi(const std::vector<uint8_t>& midi_bytes);
    ~ParsedMidi();

    //vars
    std::vector<uint8_t> m_bytes;
    int MIDI_FORMAT;
    int num_of_tracks;
    bool SMPTE_time;
    std::vector<uint8_t> m_headerChunkLength;

    // track chunk
    std::vector<TrackChunk> m_tracks;

    bool isTrackChunk;
    std::vector<int> track_start;
    std::vector<int> trackLength;


    int ticksInQuarterNote;
    const juce::String CHUNK_TITLE_HEX = "4D54726B";
    const int HEADER_END = 14;
    private:

    void createMidiChunk(int trackStart);
    bool validateTrackChunk(int trackStart);
    void establishSMPTE();
    void printMidiInfo();
    void get_length_of_tracks();
    int check_if_next_track_valid(int byte, int track_length);

};


#endif //MYPROJECT_PARSEDMIDI_H