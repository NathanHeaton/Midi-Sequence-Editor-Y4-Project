//
// Created by nathan on 21/11/2025.
//

#ifndef MYPROJECT_TRACKCONTENT_H
#define MYPROJECT_TRACKCONTENT_H


class TrackContent : public juce::Component
{
public:
    TrackContent()
    {
        addAndMakeVisible(title);
        addAndMakeVisible(muteButton);
        addAndMakeVisible(soloButton);

        title.setText("Track", juce::dontSendNotification);
        muteButton.setButtonText("M");
        soloButton.setButtonText("S");
    }

    void resized() override
    {
        juce::FlexBox row;
        row.flexDirection = juce::FlexBox::Direction::row;
        row.alignItems = juce::FlexBox::AlignItems::center;

        row.items.add(juce::FlexItem(title).withFlex(1.0f));
        row.items.add(juce::FlexItem(muteButton).withWidth(40));
        row.items.add(juce::FlexItem(soloButton).withWidth(40));

        row.performLayout(getLocalBounds());
    }

private:
    juce::Label title;
    juce::TextButton muteButton {"Mute"};
    juce::TextButton soloButton {"Solo"};
};

#endif //MYPROJECT_TRACKCONTENT_H