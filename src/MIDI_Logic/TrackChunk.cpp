//
// Created by nathan on 17/10/2025.
//

#include "TrackChunk.h"

#include <filesystem>


TrackChunk::TrackChunk(std::vector<uint8_t>& bytes) : m_trackBytes(bytes), m_midiReader(bytes){
    createNoteEvents();
    printMidiInfo();
}

TrackChunk::~TrackChunk() {

}


void TrackChunk::createNoteEvents() {

    while (!m_endofTrack && m_midiReader.index < m_trackBytes.size()) {
        DBG("track index: "<<m_midiReader.index);
        uint32_t delta = m_midiReader.readVariableLength();
        DBG("delta time: " << static_cast<int>(delta));
        if (m_midiReader.peak() == 0xFF) {
            handleMetaEvent(delta);
        }
        else if (m_midiReader.peak() == 0xF0 ||m_midiReader.peak()== 0xF7) {
            DBG("yeah sysex event");
        }
        else {
            handleInstrumentEvent(delta);
        }
    }
}

void TrackChunk::handleMetaEvent(uint32_t delta) {
    uint8_t status = m_midiReader.readNext();
    uint8_t event = m_midiReader.readNext();
    uint32_t length = 0;
    if (event == 0x00 || event == 0x20) {
        length = m_midiReader.readVariableLength();// finds the length of the data
    }
    else if (event == 0x51) {

    }
    else if (event == 0x2F) {
        m_endofTrack = true;
        return;
    }
    length = m_midiReader.readVariableLength();// finds the length of the data
    DBG("length"<< (static_cast<int>(length)));
    std::vector<uint8_t> dataBytes = m_midiReader.readNextN(length);// adds all of the meta event data

    MidiEvent midi_event(delta, status,event, length,dataBytes );
    Events.push_back(midi_event);

}

void TrackChunk::handleInstrumentEvent(uint32_t delta) {
    uint8_t status = 0;

    if ((m_midiReader.peak() & 0x80) != 0) {
        status = m_midiReader.readNext();
        m_midiReader.m_runningStatus = status;
    }
    else {
        status = m_midiReader.m_runningStatus;
    }

    uint8_t note = m_midiReader.readNext();
    uint8_t velocity = m_midiReader.readNext();
    DBG("note event: "<<status<< " " << note<<" "<<velocity);
    MidiEvent midi_event(delta, status, note, velocity);
    Events.push_back(midi_event);
}


void TrackChunk::printMidiInfo() {
    for (MidiEvent i : Events) {
        i.printEventData();
    }

}
