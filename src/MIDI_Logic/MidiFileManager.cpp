#include "MidiFileManager.h"

#include <span>

MidiFileManager::MidiFileManager()
{

}

MidiFileManager::~MidiFileManager()
{
}

std::string MidiFileManager::getMidiFile(){return m_fileName;}

void MidiFileManager::loadFile(juce::File& file)
{
    DBG("sending change message");
    sendActionMessage("fileLoaded");

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

        std::vector<uint8_t> bytes( // creates a std::vector of bytes contatined in the file
            static_cast<const uint8_t*>(data.getData()),
            static_cast<const uint8_t*>(data.getData()) + data.getSize()
        );
        DBG(data.getSize());
        //for (size_t i = 0; i < data.getSize()/8; ++i)
            //DBG(i << " 0x" << generate8Bytes(bytes,i));

        if (vector_to_HexString(vector_slice(bytes,0,4)) == "4D5468640") {
            DBG("valid file");
        }
        else {
            DBG("file bytes are invalid: "<<vector_to_HexString(vector_slice(bytes,0,4)));
        }
        m_ParsedMidi.emplace_back(bytes);
        sendActionMessage("midiData");
    }
    else {
        DBG("file not opened");
    }
}

