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

        std::vector<uint8_t> bytes( // creates an std::vector of bytes contatined in the file
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
        ParsedMidi newMidiData(bytes);
    }
    else {
        DBG("file not opened");
    }

}

juce::String MidiFileManager::generate8Bytes(auto* bytes,int numBytes) {
    juce::String string8 = "";
    int batch = numBytes*8;
    for (int i = 0+batch; i < 8+batch; ++i) {
        string8 +=  (" " + juce::String::toHexString(static_cast<const uint8_t *>(bytes)[i]));
    }
    return string8;
}

