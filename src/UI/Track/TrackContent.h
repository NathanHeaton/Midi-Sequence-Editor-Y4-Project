//
// Created by nathan on 21/11/2025.
//

#include "../../Theme.h"
#include <juce_gui_extra/juce_gui_extra.h>

#ifndef MYPROJECT_TRACKCONTENT_H
#define MYPROJECT_TRACKCONTENT_H

class TrackContent : public juce::Component
{
public:

    static constexpr float barWidth = 200.0f;
    static constexpr int beatsPerBar = 4;
    static constexpr float tempDivision = barWidth / beatsPerBar;

    // --- Constructor ---
    TrackContent()
    {
        setSize(3000, 170);
    }

private:
    std::vector<std::unique_ptr<juce::Label>> labelComponents;

};

#endif //MYPROJECT_TRACKCONTENT_H