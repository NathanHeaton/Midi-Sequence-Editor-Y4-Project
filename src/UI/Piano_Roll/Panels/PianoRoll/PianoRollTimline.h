//
// Created by nathan on 21/11/2025.
//

#include "../../../../Misc/MyColours.h"
#include <juce_gui_extra/juce_gui_extra.h>

class PianoRollTimeline : public juce::Component
{
public:

    static constexpr float barWidth = 200.0f;
    static constexpr int beatsPerBar = 4;
    static constexpr float tempDivision = barWidth / beatsPerBar;

    // --- Constructor ---
    PianoRollTimeline()
    {
        setSize(3000, 800);
    }

    // --- Component Painting ---
    void paint(juce::Graphics& g) override {

        makeGrid(g);
        g.setColour(MyColours::outline);
        g.drawRoundedRectangle(getLocalBounds().toFloat(), 0.0f, 2.0f);
    }

    // --- Grid Drawing Logic ---
    void makeGrid(juce::Graphics& g) {
        bool alternateBar = false;
        const int gridStartY = 20;
        for (float x = 0.0f; x < getWidth(); x += barWidth)
        {
            if (alternateBar) {
                g.setColour(MyColours::background);
            } else {
                g.setColour(MyColours::backgroundAlt);
            }
            g.fillRect(x, 0.0f, barWidth, (float)getHeight());
            alternateBar = !alternateBar;
            g.setColour(MyColours::outline);
            g.drawLine(x, 0.0f, x, (float)getHeight(), 2.0f);

            // Draw beat lines within the bar
            g.setColour(MyColours::altOutline);
            for (int j = 1; j < beatsPerBar; j++) {
                float xPosition = (float)j * tempDivision + x;
                g.drawLine(xPosition, (float)gridStartY, xPosition, (float)getHeight(), 1.0f);
            }
        }

        // Draw the horizontal timeline line
        g.setColour(MyColours::outline);
        g.drawLine(0.0f, (float)gridStartY, (float)getWidth(), (float)gridStartY, 2.0f);
    }

    // --- Component Layout and Resizing ---
    void resized() override
    {
        const int numBarsNeeded = static_cast<int>(std::ceil(getWidth() / barWidth));
        while (labelComponents.size() > numBarsNeeded)
        {
            removeChildComponent(labelComponents.back().get());
            labelComponents.pop_back();
        }

        for (int i = labelComponents.size(); i < numBarsNeeded; i++)
        {
            auto newLabel = std::make_unique<juce::Label>(
                "timline_" + std::to_string(i),
                std::to_string(i)
            );
            addAndMakeVisible(*newLabel);
            labelComponents.push_back(std::move(newLabel));
        }
        juce::FlexBox row;
        row.flexDirection = juce::FlexBox::Direction::row;
        row.alignItems = juce::FlexBox::AlignItems::stretch;

        for (const auto& lblPtr : labelComponents)
        {
            row.items.add(juce::FlexItem(*lblPtr)
                .withWidth(barWidth)
                .withHeight(25.0f));
        }
        row.performLayout(getLocalBounds().withHeight(25)); // Only layout the top 25px for labels
    }

private:
    std::vector<std::unique_ptr<juce::Label>> labelComponents;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRollTimeline)
};
