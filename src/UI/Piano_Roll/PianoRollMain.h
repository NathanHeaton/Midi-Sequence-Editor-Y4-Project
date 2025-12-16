//
// Created by nathan on 24/11/2025.
//
#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../Theme.h"
#include "Panels/Tools.h"
#include "Panels/PianoRollComponent.h"
#include <iostream>


class PianoRollMain : public juce::Component
{
public:

    Tools tools;
    PianoRollComponent pianoRollEditor;

    PianoRollMain() {
        addAndMakeVisible(&tools);
        addAndMakeVisible(&pianoRollEditor);
        addAndMakeVisible(label);
        label.setText("MidiPattern_1", juce::dontSendNotification);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillRoundedRectangle(getLocalBounds().toFloat(),0.0f);
        g.drawRoundedRectangle(getLocalBounds().toFloat(),0,2);
    }

    void resized() override {
        juce::FlexBox column;
        column.flexDirection = juce::FlexBox::Direction::column;
        column.alignItems = juce::FlexBox::AlignItems::flexStart;
        column.items.add(juce::FlexItem(label).withWidth(getWidth()).withHeight(40).withMargin(8));
        column.items.add(juce::FlexItem(tools).withWidth(getWidth()).withHeight(40).withMargin(8));
        column.items.add(juce::FlexItem(pianoRollEditor).withWidth(getWidth()).withFlex(1.0f).withMargin(12));

        column.performLayout(getLocalBounds());
    }
    juce::Label label;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRollMain)
};
