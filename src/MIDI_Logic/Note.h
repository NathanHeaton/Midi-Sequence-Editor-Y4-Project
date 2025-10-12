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

    Note(int pitch, float start, float duration, int velocity, float pan);
    ~Note();
    
    void change_pitch(int pitch);
    void change_start(float start);
    void change_duration(float duration);
    void change_velocity(int velocity);
    void change_pan(float pan);
private:

};

