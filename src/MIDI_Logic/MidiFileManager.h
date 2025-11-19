#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <iostream>
#include "ParsedMidi.h"
#include  "../utils.h"

class MidiFileManager : public juce::ActionBroadcaster
{
public:
    // variables
    juce::File m_midiFile;
    std::string m_fileName;
    std::vector<ParsedMidi> m_ParsedMidi;


    // functions
    MidiFileManager();
    ~MidiFileManager();
    std::string getMidiFile();
    void loadFile(juce::File& file);
    void readMidiFile();
    juce::String generate8Bytes(auto* bytes, int numBytes);

private:
    void createMidiProjectFolder();
    void midiFileParsing(auto* midi_bytes);


};

