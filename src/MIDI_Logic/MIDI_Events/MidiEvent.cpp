//
// Created by nathan on 01/11/2025.
//

#include "MidiEvent.h"

MidiEvent::MidiEvent(uint8_t meta_type, uint8_t v_length, std::vector<uint8_t> meta_bytes ) {
    type = META;
}

MidiEvent::MidiEvent(uint32_t delta, uint8_t status, uint8_t pitch, uint8_t velocity) {
    type = NOTE;

}

