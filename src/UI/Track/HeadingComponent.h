//
// Created by nathan on 21/11/2025.
//
#include "../../Misc/MyColours.h"
#ifndef MYPROJECT_HEADINGCOMPONENT_H
#define MYPROJECT_HEADINGCOMPONENT_H


class HeadingComponent : public juce::Component
{
public:
    HeadingComponent()
    {
        addAndMakeVisible(title);
        addAndMakeVisible(muteButton);
        addAndMakeVisible(soloButton);
        addAndMakeVisible(instrumentButton);
        addAndMakeVisible(effectButton);
        addAndMakeVisible(spacer);

        title.setEditable(true);
        title.setText("Track", juce::sendNotification);
        //muteButton.setButtonText("M");
        //soloButton.setButtonText("S");
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(MyColours::background);
        g.setColour(MyColours::outline);
        g.drawRoundedRectangle(getLocalBounds().toFloat(),0,2);
    }

    void resized() override
    {
        const int padding = 12;
        const float gap = .5;
        auto Bounds = getLocalBounds().reduced(padding);
        // flex box ===============
        juce::FlexBox column;
        column.flexDirection = juce::FlexBox::Direction::column;
        column.alignItems = juce::FlexBox::AlignItems::stretch;

        column.items.add(juce::FlexItem(title).withFlex(1.0f));
        column.items.add(juce::FlexItem(spacer).withFlex(gap));
        column.items.add(juce::FlexItem(muteButton).withFlex(1.0f));
        column.items.add(juce::FlexItem(spacer).withFlex(gap));
        column.items.add(juce::FlexItem(instrumentButton).withFlex(1.0f));

        //column.items.add(juce::FlexItem(soloButton).withWidth(.5f).withHeight(30));
        //column.items.add(juce::FlexItem(effectButton).withWidth(.5f).withHeight(30));

        column.performLayout(Bounds);

        juce::Rectangle<int> row1 = muteButton.getBounds();
        soloButton.setBounds(row1.withWidth(row1.getWidth() * 0.5f));
        muteButton.setBounds(row1.withTrimmedLeft(row1.getWidth() * 0.5f));

        juce::Rectangle<int> row2 = instrumentButton.getBounds();
        instrumentButton.setBounds(row2.withWidth(row2.getWidth() * 0.5f));
        effectButton.setBounds(row2.withTrimmedLeft(row2.getWidth() * 0.5f));
    }

private:
    juce::Label title;
    juce::TextButton muteButton {"Mute"};
    juce::TextButton soloButton {"Solo"};
    juce::TextButton instrumentButton {"Instrument"};
    juce::TextButton effectButton {"Effect"};
    juce::Component spacer;
};

#endif //MYPROJECT_HEADINGCOMPONENT_H