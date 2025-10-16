#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <iostream>

class MidiFileManager : public juce::ChangeBroadcaster
{
public:
    // variables
    juce::File m_midiFile;
    std::string m_fileName;

    // functions
    MidiFileManager();
    ~MidiFileManager();
    std::string getMidiFile();
    void loadFile(juce::File& file);
    void readMidiFile();
    MidiFileManager::generate8Bytes(bytes)

private:
    void createMidiProjectFolder();


};

