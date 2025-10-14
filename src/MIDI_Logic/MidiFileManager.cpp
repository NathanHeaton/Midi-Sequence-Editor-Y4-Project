#include "MidiFileManager.h"

MidiFileManager::MidiFileManager()
{
}

MidiFileManager::~MidiFileManager()
{
}

std::string MidiFileManager::getMidiFile(){return m_fileName;}

void MidiFileManager::loadFile(juce::File& file)
{
    createMidiProjectFolder();

    juce::File copyPlace("midi_files/" + file.getFileName());
    m_fileName = file.getFileName().toStdString();

    file.copyFileTo(copyPlace);
    m_midiFile = file;
    sendChangeMessage();

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