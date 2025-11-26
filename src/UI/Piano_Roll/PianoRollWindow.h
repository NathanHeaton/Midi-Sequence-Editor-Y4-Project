#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../Misc/MyColours.h"
#include "PianoRollComponent.h"

#include <iostream>


class PianoRollWindow : public juce::DocumentWindow
{
public:
    PianoRollWindow() : juce::DocumentWindow(
        "Piano Roll", MyColours::background, juce::DocumentWindow::allButtons,
        true)
    {
        setContentOwned(new PianoRollComponent(), true);

        setUsingNativeTitleBar (true);
        centreWithSize(300,300);

        setVisible(true);
    }

    void closeButtonPressed() override
    {
        delete this;
    }
private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRollWindow)
};