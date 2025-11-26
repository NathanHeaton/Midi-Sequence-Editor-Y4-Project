#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../Misc/MyColours.h"
#include "TopControls.h"
#include "../Track/PatternViewport.h"
#include "../Track/HeadingComponent.h"
#include <iostream>


class Arranger: public juce::Component,
                public  juce::Button::Listener
{
public:

    TopControls topControls;
    PatternViewport patternViewport;

    // track controls
    int numTracks = 1;
    std::vector<std::unique_ptr<HeadingComponent>> trackComponents;

    
    Arranger() {
        auto testTrack = std::make_unique<HeadingComponent>();
        addAndMakeVisible(*testTrack);
        trackComponents.push_back(std::move(testTrack));

        addAndMakeVisible(topControls);
        addAndMakeVisible(patternViewport);
        // button listeners
        topControls.addTrackButton.addListener(this);
    }


    void paint(juce::Graphics& g) override
    {
        g.setColour(MyColours::background);
        g.fillRoundedRectangle(getLocalBounds().toFloat(),0.0f);
    }

    void resized() override {
        juce::FlexBox flex;
        flex.flexDirection = juce::FlexBox::Direction::column;
        flex.alignItems = juce::FlexBox::AlignItems::flexStart;

        flex.items.add(juce::FlexItem(topControls).withWidth(getWidth()).withHeight(30));


        for (auto& track : trackComponents) {
            flex.items.add(juce::FlexItem(*track).withHeight(150).withWidth(350));
        }

        flex.performLayout(getLocalBounds());

        patternViewport.setBounds(getLocalBounds().withHeight(350).reduced(20).withTrimmedTop(130).withTrimmedLeft(300));

    }

    void buttonClicked(juce::Button* button) override {
        if (button == &topControls.addTrackButton) {
            DBG("adding track");
            auto testTrack = std::make_unique<HeadingComponent>();
            addAndMakeVisible(*testTrack);
            trackComponents.push_back(std::move(testTrack));
            resized();
        }
    }


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arranger)
};
