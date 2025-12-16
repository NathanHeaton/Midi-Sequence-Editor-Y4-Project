//
// Created by nathan on 21/11/2025.
//

#include "../../../../Theme.h"
#include <juce_gui_extra/juce_gui_extra.h>

class PianoKeys : public juce::Component
{
public:

    static constexpr float barWidth = 200.0f;
    static constexpr int beatsPerBar = 4;
    static constexpr float tempDivision = barWidth / beatsPerBar;
    static const int WHITE_KEYS = 52;
    static const int WHITE_KEY_HEIGHT = 24;
    static const int BLACK_KEYS = 26;

    // --- Constructor ---
    PianoKeys()
    {
        setSize(64, 64*WHITE_KEY_HEIGHT);
    }

    // --- Component Painting ---
    void paint(juce::Graphics& g) override {
        drawPianoRoll(g);
    }


    void drawPianoRoll(juce::Graphics& g) {

        for (int i = 0; i < WHITE_KEYS; i++) {
            g.fillRect(0,i*WHITE_KEY_HEIGHT,64,WHITE_KEY_HEIGHT-2);
            g.drawRoundedRectangle(getLocalBounds().toFloat(), 0.0f, 2.0f);
        }
    }


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoKeys)
};
