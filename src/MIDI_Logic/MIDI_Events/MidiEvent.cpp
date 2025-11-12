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




