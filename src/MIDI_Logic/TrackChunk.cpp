//
// Created by nathan on 17/10/2025.
//

#include "TrackChunk.h"

#include <filesystem>

#include "juce_gui_extra/misc/juce_PushNotifications.h"


TrackChunk::TrackChunk(std::vector<uint8_t>& bytes) : m_trackBytes(bytes), m_midiReader(bytes){
    //createNoteEvents();
}

TrackChunk::~TrackChunk() {

}


void TrackChunk::createNoteEvents() {

    while (!m_endofTrack && m_midiReader.index < m_trackBytes.size()) {
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
    uint32_t length = m_midiReader.readVariableLength();// finds the length of the data
    std::vector<uint8_t> dataBytes = m_midiReader.readNextN(length);// adds all of the meta event data
    if (event == 0x2F) {
        m_endofTrack = true;
        return;
    }
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

    MidiEvent midi_event(delta, status, note, velocity);
    Events.push_back(midi_event);
}


void TrackChunk::printMidiInfo() {
    for (MidiEvent i : Events) {

    }

}
