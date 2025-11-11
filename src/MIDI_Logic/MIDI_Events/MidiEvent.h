//
// Created by nathan on 01/11/2025.
//

#ifndef MYPROJECT_MIDIEVENT_H
#define MYPROJECT_MIDIEVENT_H
#include <cstdint>
#include <vector>

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

class MidiEvent {
    public:
    MidiEventType type;

    struct Note {
        uint8_t m_pitch;
        uint8_t m_velocity;
    };
    struct Program_Cha {
        uint8_t m_program_number;
    };
    struct Control_Cha {
        uint8_t m_controller;
        uint8_t m_value;
    };
    struct Meta_Ev {
        uint8_t m_meta_type;
        uint32_t m_v_length;
        std::vector<uint8_t> m_meta_bytes;
    };
    struct Pitch_Be {
        uint8_t m_value;
    };
    struct Poly_Af {
        uint8_t m_pressure;
        uint8_t m_pitch;
    };
    struct Poly_Cha {
        uint8_t m_pressure;
    };
    struct SysEx {
        std::vector<uint8_t> m_meta_bytes;
    };

    uint8_t m_channel;
    uint32_t m_delta;
    uint8_t m_status;

    // structs
    Note m_note;
    Program_Cha m_program_cha;
    Control_Cha m_control_cha;
    Pitch_Be m_pitch_bend;
    SysEx m_sysex;
    Meta_Ev m_meta_event;
    Poly_Af m_poly_aftertouch;
    Poly_Cha m_channel_aftertouch;

    // note data
    MidiEvent(uint32_t delta, uint8_t status, Note a, uint8_t channel);// for note events
    //meta events
    MidiEvent(uint32_t delta, uint8_t status, Meta_Ev event); // for meta events
    // program change
    MidiEvent(uint32_t delta, uint8_t status, Program_Cha change, uint8_t channel);
    // control change
    MidiEvent(uint32_t delta, uint8_t status, Control_Cha control_cha, uint8_t channel);
    // pitchbend
    MidiEvent(uint32_t delta, uint8_t status, Pitch_Be val, uint8_t channel);
    //poly aftertouch
    MidiEvent(uint32_t delta, uint8_t status, Poly_Af val, uint8_t channel);
    //channel aftertouch
    MidiEvent(uint32_t delta, uint8_t status, Poly_Cha val, uint8_t channel);
    // sysex
    MidiEvent(uint32_t delta, SysEx bytes);


    void printEventData();


};


#endif //MYPROJECT_MIDIEVENT_H