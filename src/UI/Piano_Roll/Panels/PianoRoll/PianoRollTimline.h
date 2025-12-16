//
// Created by nathan on 21/11/2025.
//

#include "../../../../Theme.h"
#include <juce_gui_extra/juce_gui_extra.h>
#include "../../../../SessionData.h"

class PianoRollTimeline : public juce::Component,
                           public SessionData::Listener
{
public:

    float barWidth = 1;
    int beatsPerBar = 1;
    float tempDivision = barWidth / static_cast<float>(beatsPerBar);


    // --- Constructor ---
    PianoRollTimeline()
    {
        setSize(3000, 800);
        SessionData::instance().listeners.add(this);
    }

    void barWidthChanged(float newBarWidth) override {
        barWidth = newBarWidth;
        tempDivision = barWidth / static_cast<float>(beatsPerBar);

        repaint();
    }

private:
    std::vector<std::unique_ptr<juce::Label>> labelComponents;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRollTimeline)
};
