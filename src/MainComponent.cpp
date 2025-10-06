#include "MainComponent.h"
#include "MyColours.h"

MainComponent::MainComponent()
{
    setSize (600, 400);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (MyColours::background);

    g.setFont (juce::FontOptions (30.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Test Project", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
}
