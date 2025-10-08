#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <iostream>

class MidiFileManager
{
public:
    // variables
    juce::File m_midiFile;

    // functions
    MidiFileManager();
    ~MidiFileManager();
    std::string getMidiFile();
    void loadFile(juce::File& file);
    void createMidiProjectFolder();

private:
    std::string findMidiFile();


};

