
#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../../Misc/MyColours.h"
#include "PianoKeys.h"

class PianoViewport: public juce::Component
{
public:

    PianoKeys pianoRoll;

    PianoViewport() {
        addAndMakeVisible(viewport2);
        addAndMakeVisible(pianoRoll);
        viewport2.setViewedComponent(&pianoRoll,false);

        viewport2.setScrollBarPosition(true,false);
    }

    void resized() override {
        juce::Rectangle<int> Area = getLocalBounds();

        viewport2.setBounds(Area);
    }

    juce::Viewport viewport2;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoViewport)
};
