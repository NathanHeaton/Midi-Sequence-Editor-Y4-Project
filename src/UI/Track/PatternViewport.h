//
// Created by nathan on 23/11/2025.
//

#ifndef MYPROJECT_PATTERNVIEWPORT_H
#define MYPROJECT_PATTERNVIEWPORT_H


#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../Theme.h"
#include <iostream>
#include "TrackContent.h"

class PatternViewport: public juce::Component
{
public:

    TrackContent trackContent;

    PatternViewport() {
        addAndMakeVisible(viewport);
         viewport.setViewedComponent(&trackContent,false);

        viewport.setScrollBarPosition(1,0);
    }


    void resized() override {
        juce::Rectangle<int> Area = getLocalBounds();
        viewport.setBounds(Area);
    }

    juce::Viewport viewport;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatternViewport)
};

#endif //MYPROJECT_PATTERNVIEWPORT_H