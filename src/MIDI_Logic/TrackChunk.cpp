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
    DBG(vector_to_HexString(m_trackBytes));
    while (!m_endofTrack && m_midiReader.index < m_trackBytes.size()) {
        DBG("track index: "<<m_midiReader.index);
        uint32_t delta = m_midiReader.readVariableLength();
        DBG("delta time: " << static_cast<int>(delta));
        DBG("type:"<< m_midiReader.peak());
        uint8_t status = m_midiReader.peak();
        uint8_t eventType = status & 0xF0;
        uint8_t channel   = status & 0x0F;

        // SYSTEM / META
        if (status == 0xFF) {
            handleMetaEvent(delta);
        }
        else if (status == 0xF0 || status == 0xF7) {
            handleSysExEvent(delta);
        }

        // CHANNEL EVENTS
        else if (eventType == 0x80) {
            // Note Off
            handleNoteOff(delta, channel);
        }
        else if (eventType == 0x90) {
            // Note On
            handleNoteOn(delta, channel);
        }
        else if (eventType == 0xA0) {
            // Polyphonic Aftertouch
            handlePolyAftertouch(delta, channel);
        }
        else if (eventType == 0xB0) {
            // Control Change
            handleControlChange(delta, channel);
        }
        else if (eventType == 0xC0) {
            // Program Change
            handleProgramEvent(delta, channel);
        }
        else if (eventType == 0xD0) {
            // Channel Aftertouch
            handleChannelAftertouch(delta, channel);
        }
        else if (eventType == 0xE0) {
            // Pitch Bend
            handlePitchBend(delta, channel);
        }

        // ------------------
        // UNKNOWN / UNHANDLED
        // ------------------
        else {
            DBG("Unknown event: " << m_midiReader.peak() << (int)status);
            // You can skip or handle gracefully here
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

void TrackChunk::handleSysExEvent(uint32_t delta) {
}

void TrackChunk::handleNoteOff(uint32_t delta, uint8_t channel) {

}

void TrackChunk::handleNoteOn(uint32_t delta, uint8_t channel) {
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

void TrackChunk::handlePolyAftertouch(uint32_t delta, uint8_t channel) {
}

void TrackChunk::handleControlChange(uint32_t delta, uint8_t channel) {
}

void TrackChunk::handleProgramEvent(uint32_t delta, uint8_t channel) {
    uint8_t status = m_midiReader.readNext();
    uint8_t program = m_midiReader.readNext();
    DBG("program event: "<<status<<" "<< program);
    MidiEvent midi_event(delta, status, program);
    Events.push_back(midi_event);
}

void TrackChunk::handleChannelAftertouch(uint32_t delta, uint8_t channel) {
}

void TrackChunk::handlePitchBend(uint32_t delta, uint8_t channel) {
}


void TrackChunk::printMidiInfo() {
    for (MidiEvent i : Events) {
        i.printEventData();
    }

}
