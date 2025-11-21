#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../Misc/MyColours.h"
#include "HeadingComponent.h"
#include "TrackContent.h"

#include <iostream>


class TrackComponent: public juce::Component
{
public:

    HeadingComponent heading;
    TrackContent content;

    TrackComponent() {
        addAndMakeVisible(heading);
        addAndMakeVisible(content);
    }


    void paint(juce::Graphics& g) override
    {
        g.setColour(MyColours::backgroundAlt);
        g.fillRoundedRectangle(getLocalBounds().toFloat(),0.0f);

        g.setColour(juce::Colours::white);
        g.drawRoundedRectangle(getLocalBounds().toFloat(),0,2);
    }

    void resized() override {
        juce::FlexBox flex;
        flex.flexDirection = juce::FlexBox::Direction::row;
        flex.alignItems = juce::FlexBox::AlignItems::center;


        flex.items.add(juce::FlexItem(heading).withWidth(300).withHeight(getHeight()));
        flex.items.add(juce::FlexItem(content).withFlex(1.0f));


        flex.performLayout(getLocalBounds());
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackComponent)
};
