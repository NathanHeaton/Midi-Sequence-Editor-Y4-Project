#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../Misc/MyColours.h"
#include "../MIDI_Logic/ParsedMidi.h"
#include <iostream>


class MidiMessage: public juce::Component
{
public:
    std::string m_text{"Midi_Messages"};

    MidiMessage();

    void paint(juce::Graphics& g) override
    {
        g.setColour(MyColours::backgroundAlt);
        g.fillRoundedRectangle(getLocalBounds().toFloat(),0.0f);

        g.setColour(juce::Colours::white);
        g.drawRoundedRectangle(getLocalBounds().toFloat(),0,2);
        g.drawText (m_text, getLocalBounds(), juce::Justification::topLeft, true);
    }

    void change_text(std::string text)
    {
        m_text = text;
    }
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiMessage)
};
