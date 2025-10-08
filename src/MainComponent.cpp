#include "MainComponent.h"
#include "MyColours.h"
#include "MidiFileManager.h"


MainComponent::MainComponent()
{
    addAndMakeVisible(openFileButton);
    openFileButton.addListener(this);
    setSize (1960, 1080);
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
    openFileButton.setBounds(15,15, 100, 15);
}


void MainComponent::buttonClicked(juce::Button* button)
{
    if(button == &openFileButton)
    {
        juce::File initalDir = juce::File::getCurrentWorkingDirectory();

        auto chooser = std::make_shared<juce::FileChooser>("choose a .midi or .mid file",
                                    initalDir ,
                                    "*.mid;*");
        
        chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this, chooser](const juce::FileChooser& fc)
            {
                juce::File chosen = fc.getResult();
                if (chosen.existsAsFile())
                {
                    DBG("Async selected file: " + chosen.getFullPathName());
                    MidiFileManager::loadFile(chosen);
                }
            });

    }
}
