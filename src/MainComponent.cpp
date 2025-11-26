#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(arranger);
    addAndMakeVisible(testBox);
    addAndMakeVisible(&controlComponent);
    addAndMakeVisible(&topNavComponent);


    test_function();
    //midifileManager.addActionListener(this);


    setSize (1960, 1080);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (MyColours::background);
    g.setFont (juce::FontOptions (45.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Test Project", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    juce::FlexBox column;
    column.flexDirection = juce::FlexBox::Direction::column;
    column.alignItems = juce::FlexBox::AlignItems::center;

    column.items.add(juce::FlexItem (topNavComponent).withHeight(36).withWidth(getWidth()));
    column.items.add(juce::FlexItem (controlComponent).withHeight(92).withWidth(getWidth()));
    column.items.add(juce::FlexItem (arranger).withHeight(800).withWidth(getWidth()));

    column.performLayout(getLocalBounds());
}


