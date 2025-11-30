
#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <functional>

class PianoViewport:  public juce::Viewport
{
public:

    PianoViewport() = default;

    std::function<void(int)> onScrollY;

    void scrollBarMoved(juce::ScrollBar *scrollBarThatHasMoved, double newRangeStart) override {
        if (onScrollY) onScrollY(getViewPositionY());
    }



};

