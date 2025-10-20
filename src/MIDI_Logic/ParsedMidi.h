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


class ParsedMidi {
    public:
    ParsedMidi(const std::vector<uint8_t>& midi_bytes);
    ~ParsedMidi();

    //vars
    std::vector<uint8_t> m_bytes;
    int MIDI_FORMAT;
    int tracks;
    bool SMPTE_time;

    int ticksInQuarterNote;
    private:
    void createMidiChunk();
    void establishSMPTE();
    void printMidiInfo();

};


#endif //MYPROJECT_PARSEDMIDI_H