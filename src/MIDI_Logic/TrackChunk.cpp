//
// Created by nathan on 17/10/2025.
//

#include "TrackChunk.h"

#include <filesystem>

TrackChunk::TrackChunk(std::vector<uint8_t>& bytes) : m_trackBytes(bytes), m_midiReader(bytes){
    std::cout << "ParsedMidi 3" << std::endl;
    createNoteEvents();
    //printMidiInfo();
}

TrackChunk::~TrackChunk() {

}

void TrackChunk::createNoteEvents() {
    std::cout << "Track 5" << std::endl;
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
        else {
        }
    }
}

void TrackChunk::handleMetaEvent(uint32_t delta, uint8_t status) {
    std::cout << "meta" << std::endl;
    uint8_t event = m_midiReader.readNext();
    uint32_t length = 0;
    std::cout << "reading len" << std::endl;

    if (event == 0x00 || event == 0x20) {
        length = m_midiReader.readVariableLength();// finds the length of the data
    }
    else if (event == 0x51) {
        length = m_midiReader.readVariableLength();// finds the length of the data
    }
    else if (event == 0x2F) {
        std::cout << "ending track" << std::endl;
        m_endofTrack = true;
        return; // this line
    }
    else {
        length = m_midiReader.readVariableLength();// finds the length of the data
    }
    std::vector<uint8_t> dataBytes = m_midiReader.readNextN(length);// adds all the meta event data

    MidiEvent midi_event(delta, status,{event, length,dataBytes} );
    std::cout << "pushing events" << std::endl;
    Events.push_back(midi_event);
    std::cout << "pushed events" << std::endl;
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


std::string TrackChunk::printMidiInfo() {
    std::string info;
    for (MidiEvent i : Events) {
        info += i.getMidiData();
    }
    return info;
}