//
// Created by nathan on 01/11/2025.
//

#include "MidiEvent.h"

#include "juce_core/system/juce_PlatformDefs.h"

MidiEvent::MidiEvent(uint32_t delta, uint8_t status, Meta_Ev event)
    : m_delta(delta),
      m_status(status),
      m_channel(0),
      m_meta_event(event),
      m_note{},
      m_program_cha{},
      m_control_cha{},
      m_pitch_bend{},
      m_poly_aftertouch{},
      m_channel_aftertouch{},
      m_sysex{},
      type(META)
{}

MidiEvent::MidiEvent(uint32_t delta, uint8_t status, Note a, uint8_t channel)
    : m_delta(delta),
      m_status(status),
      m_channel(channel),
      m_note(a),
      m_meta_event{},
      m_program_cha{},
      m_control_cha{},
      m_pitch_bend{},
      m_poly_aftertouch{},
      m_channel_aftertouch{},
      m_sysex{}
{
    if ((status & 0xF0) == 0x90) {
        type = NOTE_ON;
    } else if ((status & 0xF0) == 0x80) {
        type = NOTE_OFF;
    } else {
        DBG("unknown status "<< (int)status);
    }
}

MidiEvent::MidiEvent(uint32_t delta, uint8_t status, Program_Cha change, uint8_t channel)
    : m_delta(delta),
      m_status(status),
      m_channel(channel),
      m_program_cha(change),
      m_meta_event{},
      m_note{},
      m_control_cha{},
      m_pitch_bend{},
      m_poly_aftertouch{},
      m_channel_aftertouch{},
      m_sysex{},
      type(PROGRAM_CHANGE)
{}


MidiEvent::MidiEvent(uint32_t delta, uint8_t status, Control_Cha control, uint8_t channel)
    : m_delta(delta),
      m_status(status),
      m_channel(channel),
      m_control_cha(control),
      m_meta_event{},
      m_note{},
      m_program_cha{},
      m_pitch_bend{},
      m_poly_aftertouch{},
      m_channel_aftertouch{},
      m_sysex{},
      type(CONTROL_CHANGE)
{}


MidiEvent::MidiEvent(uint32_t delta, uint8_t status, Pitch_Be value, uint8_t channel)
    : m_delta(delta),
      m_status(status),
      m_channel(channel),
      m_pitch_bend(value),
      m_meta_event{},
      m_note{},
      m_program_cha{},
      m_control_cha{},
      m_poly_aftertouch{},
      m_channel_aftertouch{},
      m_sysex{},
      type(PITCH_BEND)
{}

MidiEvent::MidiEvent(uint32_t delta, uint8_t status, Poly_Af value, uint8_t channel)
    : m_delta(delta),
      m_status(status),
      m_channel(channel),
      m_poly_aftertouch(value),
      m_meta_event{},
      m_note{},
      m_program_cha{},
      m_control_cha{},
      m_pitch_bend{},
      m_channel_aftertouch{},
      m_sysex{},
      type(POLY_AFTERTOUCH)
{}

MidiEvent::MidiEvent(uint32_t delta, uint8_t status, Poly_Cha value, uint8_t channel)
    : m_delta(delta),
      m_status(status),
      m_channel(channel),
      m_channel_aftertouch(value),
      m_meta_event{},
      m_note{},
      m_program_cha{},
      m_control_cha{},
      m_pitch_bend{},
      m_poly_aftertouch{},
      m_sysex{},
      type(CHANNEL_AFTERTOUCH)
{}

MidiEvent::MidiEvent(uint32_t delta, SysEx bytes)
    : m_delta(delta),
      m_status(0xF0),//
      m_channel(0),
      m_sysex(bytes),
      m_meta_event{},
      m_note{},
      m_program_cha{},
      m_control_cha{},
      m_pitch_bend{},
      m_poly_aftertouch{},
      m_channel_aftertouch{},
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




