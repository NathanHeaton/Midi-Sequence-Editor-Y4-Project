#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
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


private:
    void createMidiProjectFolder();
    void readMidiFile();

};

