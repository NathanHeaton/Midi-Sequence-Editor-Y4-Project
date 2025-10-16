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
    sendChangeMessage();
    createMidiProjectFolder();
    juce::File copyPlace("midi_files/" + file.getFileName());
    m_fileName = file.getFileName().toStdString();
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

    if (stream.openedOk()) {
        juce::MemoryBlock data;
        stream.readIntoMemoryBlock(data);

        auto* bytes = data.getData();
        DBG(data.getSize());

        for (size_t i = 0; i < data.getSize(); ++i)
            DBG("Byte " << i << ": 0x" << generate8Bytes(bytes));


    }
    else {
        DBG("file not opened");
    }

}

juce::String MidiFileManager::generate8Bytes(auto* bytes) {
    juce::String string8 = "";
    for (size_t i = 0; i = 8; ++i) {
        string8 += juce::String::toHexString(((const uint8_t*)bytes)[i]);
    }
    return string8;
}