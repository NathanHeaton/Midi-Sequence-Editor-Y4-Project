#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

#include "../../../Misc/MyColours.h"
#include <iostream>


class Tools: public juce::Component
{
public:

    //juce::ToolbarButton toolbarButton;
    // std::unique_ptr<juce::Drawable> createIconDrawable() {
    //  return juce::Drawable::createFromImageData(
    //
    //  )
    // }
    juce::Label label;

    Tools() {
        addAndMakeVisible(label);
        // addAndMakeVisible(toolbarButton);
        label.setText("&",juce::NotificationType::dontSendNotification);
        //toolbarButton.setButtonText("D");
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(MyColours::background);
        g.fillRoundedRectangle(getLocalBounds().toFloat(),0.0f);
    }

    void resized() override {
        juce::FlexBox flex;
        flex.flexDirection = juce::FlexBox::Direction::row;
        flex.alignItems = juce::FlexBox::AlignItems::flexStart;
        flex.items.add(juce::FlexItem(label).withWidth(25).withHeight(25).withMargin(5));
        flex.items.add(juce::FlexItem(label).withWidth(25).withHeight(25).withMargin(5));
        flex.items.add(juce::FlexItem(label).withWidth(25).withHeight(25).withMargin(5));
        //flex.items.add(juce::FlexItem(toolbarButton).withWidth(25).withHeight(25).withMargin(5));
        flex.performLayout(getLocalBounds());
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tools)
};
