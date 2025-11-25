//
// Created by nathan on 21/11/2025.
//

#include "../../Misc/MyColours.h"
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
            // Draw alternating background
            if (alternateBar) {
                g.setColour(MyColours::background);
            } else {
                g.setColour(MyColours::backgroundAlt);
            }
            g.fillRect(x, 0.0f, barWidth, (float)getHeight());
            alternateBar = !alternateBar;

            // Draw bar line
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
        // 1. Determine how many labels are needed
        // Use static_cast for clear conversion from float to int
        const int numBarsNeeded = static_cast<int>(std::ceil(getWidth() / barWidth));

        // 2. Ensure the label array has the correct size
        // This is the main performance optimization: reuse existing labels
        // instead of deleting and recreating them on every resize.

        // Remove excess components
        while (labelComponents.size() > numBarsNeeded)
        {
            // removeComponent will automatically delete the owned component
            // because it is stored as a unique_ptr.
            removeChildComponent(labelComponents.back().get());
            labelComponents.pop_back();
        }

        // Add missing components
        for (int i = labelComponents.size(); i < numBarsNeeded; i++)
        {
            // Use std::make_unique for safe and modern pointer creation
            auto newLabel = std::make_unique<juce::Label>(
                "timline_" + std::to_string(i),
                std::to_string(i)
            );

            // Add the component to the parent and make it visible
            addAndMakeVisible(*newLabel);

            // Transfer ownership of the pointer to the vector
            labelComponents.push_back(std::move(newLabel));
        }

        // 3. Setup FlexBox for Layout
        juce::FlexBox row;
        row.flexDirection = juce::FlexBox::Direction::row;
        row.alignItems = juce::FlexBox::AlignItems::stretch;

        // 4. Configure FlexItems using the components in the vector
        for (const auto& lblPtr : labelComponents)
        {
            // Get the raw pointer from the unique_ptr and use it
            row.items.add(juce::FlexItem(*lblPtr)
                .withWidth(barWidth)
                .withHeight(25.0f));
        }

        // 5. Perform the Layout
        row.performLayout(getLocalBounds().withHeight(25)); // Only layout the top 25px for labels
    }

private:
    // --- Member Variables (PIMPL/Best Practice) ---
    // 🚨 Changed to std::vector of std::unique_ptr for safe memory management.
    // This is the idiomatic way in modern C++ to store heap-allocated objects
    // as it automatically handles deletion in the destructor.
    std::vector<std::unique_ptr<juce::Label>> labelComponents;

    // Note: The AlternateBar member variable is no longer needed since it's
    // handled as a local variable within makeGrid().
    // You also removed the barWidth, beatsPerBar, and tempDivision
    // members in favor of the static constexpr variables above.
};

#endif //MYPROJECT_TRACKCONTENT_H