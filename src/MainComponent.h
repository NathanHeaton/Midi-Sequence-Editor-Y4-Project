#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_events/juce_events.h>
#include "UI/boxComponent.h"
#include "UI/MidiMessageComponent.h"
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
    MidiMessage midiMessageBox;

    MainComponent();
    void paint (juce::Graphics&) override;
    void resized() override;

    void test_function()
    {
        testBox.change_text("changing Text");
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:

    juce::TextButton openFileButton { "open Midi"};
    juce::TextButton testButton {"test!"};
    juce::DrawableRectangle testAngle;
    

    void buttonClicked(juce::Button* button) override;


    void addUIAndMakeVisible();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)

};
