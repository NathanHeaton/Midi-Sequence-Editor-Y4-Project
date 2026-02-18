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

    void setPitch(uint8_t newPitch) {
        if (type == NOTE_ON||type==NOTE_OFF ) {
            std::get<Note>(m_event_data).m_pitch = newPitch;
        }
    }

    void setVelocity(uint8_t newVelocity) {
        if (type == NOTE_ON||type==NOTE_OFF ) {
            std::get<Note>(m_event_data).m_velocity = newVelocity;
        }
    }

    void setDelta(uint32_t newDelta) {
        m_delta = newDelta;
    }

    void setID(uint32_t newID) {m_ID = newID;}

    [[nodiscard]] uint8_t getPitch() const {
        if (type == NOTE_ON||type==NOTE_OFF ) {
            return std::get<Note>(m_event_data).m_pitch;
        }
        return 0;
    }

    [[nodiscard]] uint8_t getVelocity() const {
        if (type == NOTE_ON||type==NOTE_OFF ) {
            return std::get<Note>(m_event_data).m_velocity;
        }
        return 0;
    }
    [[nodiscard]] uint32_t getID() const {return m_ID;}
    [[nodiscard]] uint32_t getDelta() const {return m_delta;}
    [[nodiscard]] uint8_t getChannel() const {return m_channel;}
    [[nodiscard]] uint8_t getStatus() const {return m_status;}
    [[nodiscard]] uint32_t getAbsoluteTime() const {return m_absoluteTime;}

    [[nodiscard]] bool isNoteOn() const {if (type == NOTE_ON) return true; return false;}
    [[nodiscard]] bool isNoteOff() const {if (type == NOTE_OFF) return true; return false;}

    uint32_t m_absoluteTime;
    uint32_t m_ID;
private:
    uint8_t m_channel;
    uint32_t m_delta;
    uint8_t m_status;

};


#endif //MYPROJECT_MIDIEVENT_H