#include "MidiFileManager.h"

MidiFileManager::MidiFileManager()
{
}

MidiFileManager::~MidiFileManager()
{
}

std::string findMidiFile()
{
    return "0";
}


void MidiFileManager::loadFile(juce::File& file)
{
    createMidiProjectFolder();

    juce::File copyPlace("midi_files/" + file.getFileName());
    
    file.copyFileTo(copyPlace);
    m_midiFile = file;

    readMidiFile();
}


void MidiFileManager::createMidiProjectFolder()
{
    juce::File copyPlace("midi_files/"); // changes this later
    copyPlace.createDirectory();
}


void MidiFileManager::readMidiFile()
{
    juce::FileInputStream stream(m_midiFile);
}