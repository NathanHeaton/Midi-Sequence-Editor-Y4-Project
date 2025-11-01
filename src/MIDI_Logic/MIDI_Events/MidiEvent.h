//
// Created by nathan on 01/11/2025.
//

#ifndef MYPROJECT_MIDIEVENT_H
#define MYPROJECT_MIDIEVENT_H
#include <cstdint>
#include <vector>

enum MidiEventType {
    META,
    NOTE
};
class MidiEvent {


    public:
    MidiEventType type;

    MidiEvent(char pitch, float start, float duration, int velocity);// for note events

    MidiEvent(uint8_t meta_type, uint8_t v_length, std::vector<uint8_t> meta_bytes ); // for meta events
};


#endif //MYPROJECT_MIDIEVENT_H