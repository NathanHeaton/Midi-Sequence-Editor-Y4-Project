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


void MidiFileManager::loadFile(juce::File file)
{
    std::cout<<file.getFullPathName()<<std::endl;
}