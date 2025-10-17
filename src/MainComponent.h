#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "UI/boxComponent.h"
#include "MIDI_Logic/MidiFileManager.h"
#include <iostream>

class MainComponent final : public juce::Component,
                            private juce::Button::Listener,
                            public juce::ChangeListener
{
public:
    // Vars
    boxComponent testBox;
    MidiFileManager midifileManager;
    MainComponent();
    void paint (juce::Graphics&) override;
    void resized() override;

    void test_function()
    {
        testBox.change_text("changing Text");
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {
        if(source == &midifileManager)
        {

            testBox.change_text("setes");
        }
    }

private:

    juce::TextButton openFileButton { "open Midi"};
    juce::TextButton testButton {"test!"};
    juce::DrawableRectangle testAngle;
    

    void buttonClicked(juce::Button* button) override;

    void addUIAndMakeVisible();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)

};
