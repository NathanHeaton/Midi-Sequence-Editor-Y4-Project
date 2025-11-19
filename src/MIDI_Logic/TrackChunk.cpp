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
        uint32_t delta = m_midiReader.readVariableLength();
        uint8_t status;

        if ((m_midiReader.peak() & 0x80) != 0) {
            status = m_midiReader.readNext();
            m_midiReader.m_runningStatus = status;
        }
        else {
            status = m_midiReader.m_runningStatus;
        }

        uint8_t eventType = status & 0xF0;
        uint8_t channel   = status & 0x0F;

        // SYSTEM / META
        if (status == 0xFF) {
            handleMetaEvent(delta, status);
        }
        else if (status == 0xF0 || status == 0xF7) {
            handleSysExEvent(delta, status);
        }
        // CHANNEL EVENTS
        else if (eventType == 0x80 || eventType == 0x90) {
            // Note Off
            handleNote(delta, channel, status);
        }
        else if (eventType == 0xA0) {
            // Polyphonic Aftertouch
            handlePolyAftertouch(delta, channel, status);
        }
        else if (eventType == 0xB0) {
            // Control Change
            handleControlChange(delta, channel, status);
        }
        else if (eventType == 0xC0) {
            // Program Change
            handleProgramEvent(delta, channel, status);
        }
        else if (eventType == 0xD0) {
            // Channel Aftertouch
            handleChannelAftertouch(delta, channel, status);
        }
        else if (eventType == 0xE0) {
            // Pitch Bend
            handlePitchBend(delta, channel, status);
        }

        // ------------------
        // UNKNOWN / UNHANDLED
        // ------------------
        else {
            DBG("Unknown event: " << m_midiReader.peak() << (int)status);
        }
    }
}

void TrackChunk::handleMetaEvent(uint32_t delta, uint8_t status) {
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

    MidiEvent midi_event(delta, status,{event, length,dataBytes} );
    Events.push_back(midi_event);
}

void TrackChunk::handleSysExEvent(uint32_t delta, uint8_t status) {
    //
    //
    // SysEx sys_ex = {bytes};
    // MidiEvent midi_event(delta, status, note, channel);
    // Events.push_back(midi_event);
}

void TrackChunk::handleNote(uint32_t delta, uint8_t channel, uint8_t status) {
    uint8_t pitch = m_midiReader.readNext();
    uint8_t velocity = m_midiReader.readNext();
    Note note = {pitch, velocity};
    MidiEvent Note(delta, status, note, channel);
    Events.push_back(Note);
}

void TrackChunk::handlePolyAftertouch(uint32_t delta, uint8_t channel, uint8_t status) {
    uint8_t pressure = m_midiReader.readNext();
    uint8_t pitch = m_midiReader.readNext();
    Note poly_aftertouch = {pressure, pitch};
    MidiEvent PolyAftertouch(delta, status, poly_aftertouch, channel);
    Events.push_back(PolyAftertouch);
}

void TrackChunk::handleControlChange(uint32_t delta, uint8_t channel, uint8_t status) {
    uint8_t controller = m_midiReader.readNext();
    uint8_t value = m_midiReader.readNext();
    Control_Cha control_cha = {controller,value};
    MidiEvent ControlChange(delta, status, control_cha, channel);
    Events.push_back(ControlChange);
}

void TrackChunk::handleProgramEvent(uint32_t delta, uint8_t channel, uint8_t status) {
    uint8_t program = m_midiReader.readNext();
    Program_Cha program_cha = {program};
    MidiEvent ProgramEvent(delta, status, program_cha, channel);
    Events.push_back(ProgramEvent);
}

void TrackChunk::handleChannelAftertouch(uint32_t delta, uint8_t channel, uint8_t status) {
    uint8_t pressure = m_midiReader.readNext();
    Poly_Cha poly_cha = {pressure};
    MidiEvent ChannelAftertouch(delta, status, poly_cha, channel);
    Events.push_back(ChannelAftertouch);
}

void TrackChunk::handlePitchBend(uint32_t delta, uint8_t channel, uint8_t status) {
    uint8_t lsb_value = m_midiReader.readNext();
    uint8_t msb_value = m_midiReader.readNext();
    Pitch_Be pitch_be = {lsb_value, msb_value};
    MidiEvent PitchBend(delta, status, pitch_be, channel);
    Events.push_back(PitchBend);
}


void TrackChunk::printMidiInfo() {
    // DBG("printing midi track events");
    // for (MidiEvent i : Events) {
    //     DBG(i.getMidiData());
    // }


}