//
// Created by nathan on 24/11/2025.
//
#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../../Misc/MyColours.h"

#include "PianoTimelineViewport.h"
#include "PianoViewport.h"
#include "PianoRoll/PianoRollTimline.h"
#include "PianoRoll/PianoKeys.h"

class PianoRollComponent : public juce::Component
{
public:

    PianoTimelineViewport timelineViewport;
    PianoRollTimeline timeline;
    PianoViewport pianoViewport;
    PianoKeys pianoKeys;


    PianoRollComponent() {
        addAndMakeVisible(&timelineViewport);
        addAndMakeVisible(&pianoViewport);
        addAndMakeVisible(&timeline);
        timelineViewport.setViewedComponent(&timeline);
        pianoViewport.setViewedComponent(&pianoKeys);

        timelineViewport.onScrollY = [this](int y) {
            pianoViewport.setViewPosition(0,y);
        };


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

        row.items.add(juce::FlexItem(pianoViewport).withHeight(getHeight()).withWidth(64));
        row.items.add(juce::FlexItem(timelineViewport).withHeight(getHeight()).withWidth(getWidth()));

        row.performLayout(getLocalBounds());
    }


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRollComponent)
};
