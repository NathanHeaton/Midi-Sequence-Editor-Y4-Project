#include "Note.h"

Note::Note(int pitch, float start, float duration, int velocity, float pan)
{
    m_pitch = pitch;
    m_startTime = start;
    m_duration = duration;
    m_velocity = velocity;
    m_pan = pan;
}

Note::~Note()
{
}


void Note::change_pitch(int pitch)
{

}