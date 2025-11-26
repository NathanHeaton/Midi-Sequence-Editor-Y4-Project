//
//
// Created by nathan on 23/11/2025.
//

#ifndef MYPROJECT_PianoRollEditor_H
#define MYPROJECT_PianoRollEditor_H


#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "PianoTimline.h"

class PianoTimlineViewport: public juce::Component
{
public:

    PianoRollTimeline pianoRollTimeline;

    PianoTimlineViewport() {
        addAndMakeVisible(viewport);
        addAndMakeVisible(pianoRollTimeline);
        viewport.setViewedComponent(&pianoRollTimeline,false);

        viewport.setScrollBarPosition(true,false);
    }

    void resized() override {
        juce::Rectangle<int> Area = getLocalBounds();

        viewport.setBounds(Area);
    }

    juce::Viewport viewport;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoTimlineViewport)
};

#endif //MYPROJECT_PianoRollEditor_H