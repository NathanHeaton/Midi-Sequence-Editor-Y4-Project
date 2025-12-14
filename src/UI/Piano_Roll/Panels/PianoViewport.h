
#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <functional>

class PianoViewport:  public juce::Viewport
{
public:

    PianoViewport() = default;

    std::function<void(int)> onScrollY;

    void scrollBarMoved([[maybe_unused]]juce::ScrollBar *scrollBarThatHasMoved,[[maybe_unused]] double newRangeStart) override {
        if (onScrollY) onScrollY(getViewPositionY());
    }



};

