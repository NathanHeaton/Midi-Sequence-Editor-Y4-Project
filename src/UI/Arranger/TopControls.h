#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_events/juce_events.h>

#include "../../Theme.h"
#include <iostream>


class TopControls: public juce::Component
{
public:
    juce::TextButton addTrackButton;

    TopControls() = default;

    void paint(juce::Graphics& g) override
    {
        g.fillRoundedRectangle(getLocalBounds().toFloat(),0.0f);

    }

    void resized() override {
        juce::FlexBox flex;
        flex.flexDirection = juce::FlexBox::Direction::row;
        flex.alignItems = juce::FlexBox::AlignItems::flexStart;
        flex.items.add(juce::FlexItem(addTrackButton).withWidth(100).withHeight(25).withMargin(5));
        //flex.items.add(juce::FlexItem(toolbarButton).withWidth(25).withHeight(25).withMargin(5));
        flex.performLayout(getLocalBounds());
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopControls)
};
