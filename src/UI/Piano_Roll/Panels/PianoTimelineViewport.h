//
//
// Created by nathan on 23/11/2025.
//

#ifndef MYPROJECT_PianoRollEditor_H
#define MYPROJECT_PianoRollEditor_H


#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <functional>

class PianoTimelineViewport: public juce::Viewport
{
public:

    PianoTimelineViewport() = default;

    std::function<void(int)> onScrollY;

    void scrollBarMoved(juce::ScrollBar *scrollBarThatHasMoved, double newRangeStart) override {
        if (onScrollY) {
            onScrollY(getViewPositionY());
        }
    }




};

#endif //MYPROJECT_PianoRollEditor_H