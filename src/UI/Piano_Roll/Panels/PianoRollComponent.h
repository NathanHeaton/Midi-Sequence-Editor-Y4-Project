//
// Created by nathan on 24/11/2025.
//
#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

#include "PianoTimlineViewport.h"
#include "PianoViewport.h"
#include "../../../Misc/MyColours.h"


class PianoRollComponent : public juce::Component
{
public:

    PianoTimlineViewport timeline;
    PianoViewport piano;

    PianoRollComponent() {
        addAndMakeVisible(&timeline);
        addAndMakeVisible(&piano);
        label.setText("MidiPattern_1", juce::dontSendNotification);
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(MyColours::background);
        g.fillRoundedRectangle(getLocalBounds().toFloat(),0.0f);
        g.setColour(MyColours::outline);
        g.drawRoundedRectangle(getLocalBounds().toFloat(),0,2);
    }

    void resized() override {
        juce::FlexBox row;
        row.flexDirection = juce::FlexBox::Direction::row;
        row.alignItems = juce::FlexBox::AlignItems::flexStart;

        row.items.add(juce::FlexItem(piano).withHeight(getHeight()).withWidth(64));
        row.items.add(juce::FlexItem(timeline).withHeight(getHeight()).withWidth(getWidth()));

        row.performLayout(getLocalBounds());
    }
    juce::Label label;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRollComponent)
};
