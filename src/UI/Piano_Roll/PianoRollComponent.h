//
// Created by nathan on 24/11/2025.
//
#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../Misc/MyColours.h"
#include <iostream>


class PianoRollComponent : public juce::Component
{
public:

    PianoRollComponent() {

        addAndMakeVisible(label);
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(MyColours::background);
        g.fillRoundedRectangle(getLocalBounds().toFloat(),0.0f);
        g.setColour(MyColours::outline);
        g.drawRoundedRectangle(getLocalBounds().toFloat(),0,2);
    }

    void resized() override {
        juce::FlexBox flex;
        flex.flexDirection = juce::FlexBox::Direction::row;
        flex.alignItems = juce::FlexBox::AlignItems::flexStart;
        flex.items.add(juce::FlexItem(label).withWidth(100).withHeight(25).withMargin(5));

        flex.performLayout(getLocalBounds());
    }
    juce::Label label;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRollComponent)
};
