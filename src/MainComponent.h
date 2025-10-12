#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "UI/boxComponent.h"

class MainComponent final : public juce::Component,
                            private juce::Button::Listener
{
public:
    MainComponent();

    void paint (juce::Graphics&) override;
    void resized() override;

    boxComponent testBox;

private:

    juce::TextButton openFileButton { "open Midi"};

    void buttonClicked (juce::Button* button) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)

};
