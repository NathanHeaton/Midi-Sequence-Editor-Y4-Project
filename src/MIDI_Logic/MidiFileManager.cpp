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

}


void MidiFileManager::createMidiProjectFolder()
{
    juce::File copyPlace("midi_files/"); // changes this later
    copyPlace.createDirectory();
}
