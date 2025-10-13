#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class MainComponent final : public juce::Component,
                            private juce::Button::Listener
{
public:
    MainComponent();

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::TextButton openFileButton { "open Midi"};
    juce::TextButton testButton {"test!"};
    juce::DrawableRectangle testAngle;

    void buttonClicked (juce::Button* button) override;

    void addUIAndMakeVisible();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)

};
