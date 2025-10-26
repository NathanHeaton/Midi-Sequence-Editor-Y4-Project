#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class Note
{

public:
    int m_pitch;
    float m_startTime;
    float m_duration;//maybe change to end time
    int m_velocity;
    float m_pan;

    uint8_t delta_time;
    uint8_t status;
    uint8_t event_type;
    uint8_t channel;
    uint8_t pitch;
    uint8_t velocity;
    Note(int pitch, float start, float duration, int velocity, float pan);
    ~Note();
    
    void change_pitch(int pitch);
    void change_start(float start);
    void change_duration(float duration);
    void change_velocity(int velocity);
    void change_pan(float pan);
private:

};

