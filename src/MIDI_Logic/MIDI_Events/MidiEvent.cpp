//
// Created by nathan on 01/11/2025.
//

#include "MidiEvent.h"

MidiEvent::MidiEvent(uint8_t meta_type, uint8_t v_length, std::vector<uint8_t> meta_bytes ) {
    type = META;
}

MidiEvent::MidiEvent(char pitch, float start, float duration, int velocity) {
    type = NOTE;

}

