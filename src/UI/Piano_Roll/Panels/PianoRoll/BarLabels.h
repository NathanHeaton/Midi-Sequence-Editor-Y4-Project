//
// Created by nathan on 21/11/2025.
//

#include "../../../../Theme.h"
#include <juce_gui_extra/juce_gui_extra.h>
#include "../../../../SessionData.h"

class BarLabels : public juce::Component,
public SessionData::Listener
{
public:

    float barWidth = SessionData::instance().getBarWidth();
    int beatsPerBar = SessionData::instance().getDenominator();
    float tempDivision = barWidth / static_cast<float>(beatsPerBar);

    // --- Constructor ---
    BarLabels()
    {
        setSize(3000, 30);
        SessionData::instance().listeners.add(this);
    }

    void denominatorChanged(int newDenominator) override {
        beatsPerBar = newDenominator;
        tempDivision = barWidth / static_cast<float>(beatsPerBar);
        resized();
    }

    void barWidthChanged(int newBarWidth) override {
        barWidth = newBarWidth;
        tempDivision = barWidth / static_cast<float>(beatsPerBar);
        resized();
    }




    // --- Component Painting ---
    void paint(juce::Graphics& g) override {

        g.setColour(MyColours::outline);
        g.drawRoundedRectangle(getLocalBounds().toFloat(), 0.0f, 2.0f);
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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BarLabels)
};
