#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../Misc/MyColours.h"

class boxComponent: public juce::Component
{
public:
    boxComponent();
    ~boxComponent() override;

    void paint(juce::Graphics& g) override
    {
        g.setColour(MyColours::backgroundAlt);
        g.fillRoundedRectangle(getLocalBounds().toFloat(),0.0f);

        g.setColour(juce::Colours::white);
        g.drawRoundedRectangle(getLocalBounds().toFloat(),0,2);


    }
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(boxComponent)
};
