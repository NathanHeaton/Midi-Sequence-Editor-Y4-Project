#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../Misc/MyColours.h"
#include <iostream>


class boxComponent: public juce::Component
{
public:
    std::string m_text{"test that build"};

    boxComponent();
    boxComponent(std::string text);

    void paint(juce::Graphics& g) override
    {
        g.setColour(MyColours::background);
        g.fillRoundedRectangle(getLocalBounds().toFloat(),0.0f);

        g.setColour(MyColours::outline);
        g.drawRoundedRectangle(getLocalBounds().toFloat(),0,2);
        g.drawText (m_text, getLocalBounds(), juce::Justification::centred, true);
    }

    void change_text(std::string text)
    {
         m_text = text;
        repaint();
    }
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(boxComponent)
};
