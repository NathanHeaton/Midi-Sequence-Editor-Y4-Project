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

    MainComponent();
    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {
        if(source == &midifileManager)
        {
            std::cout<<"observed change"<<std::endl;
            testBox.change_text("changing Text");
        }
    }

private:
    MidiFileManager midifileManager;
    juce::TextButton openFileButton { "open Midi"};
    juce::TextButton testButton {"test!"};
    juce::DrawableRectangle testAngle;
    

    void buttonClicked (juce::Button* button) override;

    void addUIAndMakeVisible();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)

};
