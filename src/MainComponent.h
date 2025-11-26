#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_events/juce_events.h>
#include "UI/boxComponent.h"

#include "UI/ControlPannel/ControlComponent.h"
#include  "MIDI_Logic/ParsedMidi.h"
#include "Misc/MyColours.h"
#include "UI/Top_nav/TopNavComponent.h"
#include "UI/Arranger/Arranger.h"

#include <iostream>


class MainComponent final : public juce::Component
{
public:
    // Vars
    boxComponent testBox;


    // Main Panels
    TopNavComponent topNavComponent;
    ControlComponent controlComponent;
    Arranger arranger;


    MainComponent();
    void paint (juce::Graphics&) override;
    void resized() override;

    void test_function()
    {
        testBox.change_text("changing Text");
    }


private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)

};
