//
// Created by nathan on 01/11/2025.
//

#include "MidiEvent.h"
#include "juce_core/system/juce_PlatformDefs.h"

MidiEvent::MidiEvent(uint32_t delta, uint8_t status, Meta_Ev event): m_delta(delta),
    m_status(status),
    m_event_data(event),
    m_channel(0),
    type(META)
{}

MidiEvent::MidiEvent(uint32_t delta, uint8_t status, Note event, uint8_t channel)
    : m_delta(delta),
      m_status(status),
m_event_data(event),
      m_channel(channel)
{
    if ((status & 0xF0) == 0x90) {
        type = NOTE_ON;
    } else if ((status & 0xF0) == 0x80) {
        type = NOTE_OFF;
    } else {
        DBG("unknown status "<< (int)status);
    }
}

MidiEvent::MidiEvent(uint32_t delta, uint8_t status, Program_Cha event, uint8_t channel)
    : m_delta(delta),
      m_status(status),
m_event_data(event),
      m_channel(channel),
      type(PROGRAM_CHANGE)
{}


MidiEvent::MidiEvent(uint32_t delta, uint8_t status, Control_Cha event, uint8_t channel)
    : m_delta(delta),
      m_status(status),
m_event_data(event),
      m_channel(channel),
      type(CONTROL_CHANGE)
{}


MidiEvent::MidiEvent(uint32_t delta, uint8_t status, Pitch_Be event, uint8_t channel)
    : m_delta(delta),
      m_status(status),
      m_channel(channel),
      type(PITCH_BEND)
{}

MidiEvent::MidiEvent(uint32_t delta, uint8_t status, Poly_Af event, uint8_t channel)
    : m_delta(delta),
      m_status(status),
m_event_data(event),
      m_channel(channel),
      type(POLY_AFTERTOUCH)
{}

MidiEvent::MidiEvent(uint32_t delta, uint8_t status, Poly_Cha event, uint8_t channel)
    : m_delta(delta),
      m_status(status),
m_event_data(event),
      m_channel(channel),
      type(CHANNEL_AFTERTOUCH)
{}

MidiEvent::MidiEvent(uint32_t delta, SysEx event)
    : m_delta(delta),
      m_status(0xF0),//
m_event_data(event),
      m_channel(0),
      type(SYSEX)
{}



void MidiEvent::printEventData() {
    DBG("MIDI EVENT =================")
    DBG("delta: "<< m_delta);
    DBG("status: "<< m_status);
    if (type == META) {
        DBG("meta_type: "<< m_meta_type);
        DBG("v_length: "<< m_v_length);
    }
    else if (type == NOTE_ON) {
        DBG("pitch: "<< m_pitch);
        DBG("velocity: "<< m_velocity);
    }
}

std::string MidiEvent::getMidiData() {

    std::string data;
    data = "MIDI EVENT======================== \n Delta: " + std::to_string(m_delta) + " status: " + std::format("{:X}",m_status);

    switch (type) {
        case META: {
            auto meta = std::get<Meta_Ev>(m_event_data);
            data += "\nLength: " + std::to_string(meta.m_v_length);
            //data += std::format("{:X}", meta.m_meta_bytes.at(1));
            break;
        }
        case NOTE_ON:
        case NOTE_OFF: {
            auto note = std::get<Note>(m_event_data);
            data += "\nPitch: " + std::to_string(note.m_pitch);
            data += "\nVelocity: " + std::to_string(note.m_velocity);
            break;
        }
        case PROGRAM_CHANGE: {
            auto prog = std::get<Program_Cha>(m_event_data);
            data += "\nProgram: " + std::to_string(prog.m_program_number);
            break;
        }
        case CONTROL_CHANGE: {
            auto ctrl = std::get<Control_Cha>(m_event_data);
            data += "\nController: " + std::to_string(ctrl.m_controller);
            data += "\nValue: " + std::to_string(ctrl.m_value);
            break;
        }
        case PITCH_BEND: {
            //auto pitch = std::get<Pitch_Be>(m_event_data);
            //data += "\nValue: " + std::to_string(pitch.m_msb_value)+" Value: " + std::to_string(pitch.m_lsb_value);
            break;
        }
        case POLY_AFTERTOUCH: {
            auto poly = std::get<Poly_Af>(m_event_data);
            data += "\nPressure: " + std::to_string(poly.m_pressure);
            data += "\nPitch: " + std::to_string(poly.m_pitch);
            break;
        }
        case CHANNEL_AFTERTOUCH: {
            auto chan = std::get<Poly_Cha>(m_event_data);
            data += "\nPressure: " + std::to_string(chan.m_pressure);
            break;
        }
        case SYSEX: {
            auto sysex = std::get<SysEx>(m_event_data);
            data += "\nSysEx Length: " + std::to_string(sysex.m_meta_bytes.size());
            break;
        }
    }
    data += "\nChannel: " + std::to_string(m_channel);

    return data;
}






