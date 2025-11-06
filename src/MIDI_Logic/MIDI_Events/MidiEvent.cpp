//
// Created by nathan on 01/11/2025.
//

#include "MidiEvent.h"

#include "juce_core/system/juce_PlatformDefs.h"

MidiEvent::MidiEvent(uint32_t delta, uint8_t status ,uint8_t meta_type, uint32_t v_length, std::vector<uint8_t> meta_bytes )
    : m_delta(delta), m_meta_type(meta_type),
m_v_length(v_length), m_meta_bytes(meta_bytes), m_status(status)
{
    type = META;
}

void MidiEvent::printEventData() {
    DBG("MIDI EVENT =================")
    DBG("delta: "<< m_delta);
    DBG("status: "<< m_status);
    if (type == META) {
        DBG("meta_type: "<< m_meta_type);
        DBG("v_length: "<< m_v_length);
    }
    else if (type == NOTE) {
        DBG("pitch: "<< m_pitch);
        DBG("velocity: "<< m_velocity);
    }
}

MidiEvent::MidiEvent(uint32_t delta, uint8_t status, uint8_t pitch, uint8_t velocity)
    : m_delta(delta), m_pitch(pitch), m_velocity(velocity), m_status(status)
{
    type = NOTE;

}



