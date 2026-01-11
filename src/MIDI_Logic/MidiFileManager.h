#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <iostream>
#include "ParsedMidi.h"
#include  "../utils.h"
#include  "../SessionData.h"

class MidiFileManager : public juce::ActionBroadcaster
{
public:
    // variables
    juce::File m_midiFile;
    std::string m_fileName;

    // functions
    MidiFileManager() = default;
    ~MidiFileManager() = default;

    std::string getMidiFile(){return m_fileName;}

    void loadFile(juce::File& file){
        sendActionMessage("fileLoaded");
        createMidiProjectFolder();
        juce::File copyPlace("midi_files/" + file.getFileName());
        m_fileName = file.getFileNameWithoutExtension().toStdString();
        file.copyFileTo(copyPlace);
        m_midiFile = file;
        readMidiFile();
    }


    void createMidiProjectFolder(){
        juce::File copyPlace("midi_files/"); // changes this later
        copyPlace.createDirectory();
    }


    void readMidiFile(){
        juce::FileInputStream stream(m_midiFile);
        if (stream.openedOk()) {
            juce::MemoryBlock data;
            stream.readIntoMemoryBlock(data);
            std::vector<uint8_t> bytes( // creates a std::vector of bytes contatined in the file
                static_cast<const uint8_t*>(data.getData()),
                static_cast<const uint8_t*>(data.getData()) + data.getSize()
            );
            if (vector_to_HexString(vector_slice(bytes,0,4)) == "4D5468640") {
                DBG("valid file");
            }
            else {
                DBG("file bytes are invalid: "<<vector_to_HexString(vector_slice(bytes,0,4)));
            }
            DBG("got to session call");
            SessionData::instance().addParsedMidi(bytes, m_fileName);
            DBG("got past session call");
            sendActionMessage("midiData");
        }
        else {        DBG("file not opened");    }
    }

private:

};

