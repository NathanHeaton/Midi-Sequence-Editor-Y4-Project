#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_events/juce_events.h>
#include "UI/ControlPannel/ControlComponent.h"
#include  "MIDI_Logic/ParsedMidi.h"
#include "Misc/MyColours.h"
#include "UI/Top_nav/TopNavComponent.h"
#include "UI/Arranger/Arranger.h"
#include "UI/Piano_Roll/PianoRollWindow.h"

#include <iostream>

class MainComponent final : public juce::Component,
                            public juce::ActionListener
{
public:
    // Main Panels
    TopNavComponent topNavComponent;
    ControlComponent controlComponent;
    Arranger arranger;

    MainComponent()
    {
        addAndMakeVisible(&arranger);
        addAndMakeVisible(&controlComponent);
        addAndMakeVisible(&topNavComponent);
        setSize (1960, 1080);

        controlComponent.addActionListener(this);
    }

    void paint (juce::Graphics& g) override{
        g.fillAll (MyColours::background);
        g.setFont (juce::FontOptions (45.0f));
        g.setColour (juce::Colours::white);
        g.drawText ("Test Project", getLocalBounds(), juce::Justification::centred, true);
    }

    void resized() override{
        juce::FlexBox column;
        column.flexDirection = juce::FlexBox::Direction::column;
        column.alignItems = juce::FlexBox::AlignItems::center;

        column.items.add(juce::FlexItem (topNavComponent).withHeight(36).withWidth(getWidth()));
        column.items.add(juce::FlexItem (controlComponent).withHeight(92).withWidth(getWidth()));
        column.items.add(juce::FlexItem (arranger).withHeight(800).withWidth(getWidth()));

        column.performLayout(getLocalBounds());
    }

    void actionListenerCallback(const juce::String &message) override {
        DBG("main received: " + message);
        if (message == "open piano roll") {
                if ( pianoRollWindow == nullptr) {
                    DBG("open piano roll window is a nullptr");
                    pianoRollWindow = std::make_unique<PianoRollWindow>();
                }
                else {
                    pianoRollWindow->toFront(true);
                }

        }

    }
private:
    std::unique_ptr<PianoRollWindow> pianoRollWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)

};
