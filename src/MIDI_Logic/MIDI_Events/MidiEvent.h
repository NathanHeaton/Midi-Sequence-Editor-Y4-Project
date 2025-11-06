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

    MidiEvent(uint32_t delta, uint8_t status, uint8_t pitch, uint8_t velocity);// for note events

    MidiEvent(uint32_t delta, uint8_t status ,uint8_t meta_type, uint32_t v_length, std::vector<uint8_t> meta_bytes ); // for meta events

    void printEventData();

    // vars =========================
    uint32_t m_delta;
    uint8_t m_status;
    //note specific
    uint8_t m_pitch;
    uint8_t m_velocity;
    //meta specific
    uint8_t m_meta_type;
    uint32_t m_v_length;
    std::vector<uint8_t> m_meta_bytes;


};


#endif //MYPROJECT_MIDIEVENT_H