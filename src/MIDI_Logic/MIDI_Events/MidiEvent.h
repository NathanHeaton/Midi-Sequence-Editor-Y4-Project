//
// Created by nathan on 01/11/2025.
//

#ifndef MYPROJECT_MIDIEVENT_H
#define MYPROJECT_MIDIEVENT_H
#include <cstdint>
#include <string>
#include <variant>
#include <vector>
#include "MidiStructs.h"
#include <format>

class MidiEvent {

    enum MidiEventType {
        META,
        NOTE_ON,
        NOTE_OFF,
        PROGRAM_CHANGE,
        CONTROL_CHANGE,
        PITCH_BEND,
        POLY_AFTERTOUCH,
        CHANNEL_AFTERTOUCH,
        SYSEX
    };


    public:
    MidiEventType type;

    std::variant<
        Note,
        Program_Cha,
        Control_Cha,
        Meta_Ev,
        Pitch_Be,
        SysEx,
        Poly_Af,
        Poly_Cha
    > m_event_data;

    uint8_t m_channel;
    uint32_t m_delta;
    uint8_t m_status;

    // note data
    MidiEvent(uint32_t delta, uint8_t status, Note event, uint8_t channel);// for note events
    //meta events
    MidiEvent(uint32_t delta, uint8_t status, Meta_Ev event); // for meta-events
    // program change
    MidiEvent(uint32_t delta, uint8_t status, Program_Cha event, uint8_t channel);
    // control change
    MidiEvent(uint32_t delta, uint8_t status, Control_Cha event, uint8_t channel);
    // pitchbend
    MidiEvent(uint32_t delta, uint8_t status, Pitch_Be event, uint8_t channel);
    //poly aftertouch
    MidiEvent(uint32_t delta, uint8_t status, Poly_Af event, uint8_t channel);
    //channel aftertouch
    MidiEvent(uint32_t delta, uint8_t status, Poly_Cha event, uint8_t channel);
    // sysex
    MidiEvent(uint32_t delta, SysEx event);


    void printEventData();
    std::string getMidiData();


};


#endif //MYPROJECT_MIDIEVENT_H